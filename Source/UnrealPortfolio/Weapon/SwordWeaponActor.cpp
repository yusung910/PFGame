// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"
#include "DrawDebugHelpers.h"

#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "EnemyAIController.h"

#include "MainPlayerController.h"

#include "MainGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "Components/CapsuleComponent.h"

#include "SwordWeaponActor.h"

ASwordWeaponActor::ASwordWeaponActor()
{
	PrimaryActorTick.bCanEverTick = true;

	UStaticMesh* MeshAsset;
	GetObjectAsset(MeshAsset, UStaticMesh, "StaticMesh'/Game/LordVictor/Mesh/SM_Kantana.SM_Kantana'");

	MyMesh->SetStaticMesh(MeshAsset);
	MyMesh->AttachTo(Root);

	GetObjectAsset(AttackMontage1, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Player/Combo_Attack_01_Montage.Combo_Attack_01_Montage'");

	GetObjectAsset(UpperAttackMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Player/Attack_Up_Short_Montage.Attack_Up_Short_Montage'");

	GetObjectAsset(UpperAttackFloorToJumpMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Player/Attack_Up_Floor_To_Air_Montage.Attack_Up_Floor_To_Air_Montage'");

	GetObjectAsset(AirAttackMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Player/Combo_Attack_Air_Montage.Combo_Attack_Air_Montage'");

	GetObjectAsset(DashAttackMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Player/Dash_Attack_Montage.Dash_Attack_Montage'");

	GetObjectAsset(DodgeMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Player/Dodge_Combat_Montage.Dodge_Combat_Montage'");

	GetObjectAsset(ATFAttack3Montage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Player/Attack_Air_To_Floor_03_Montage.Attack_Air_To_Floor_03_Montage'");

	//초기 공격 몽타주 세팅
	CurrentAttackMontage = AttackMontage1;

}

void ASwordWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ASwordWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	gi = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
}


void ASwordWeaponActor::Attack()
{
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(GetOwner());
	//공격 시도
	if (!MyPlayer->GetIsAttack())
	{
		if (MyPlayer->GetIsAirPosition() &&
			!fPlayerInput.isSPress &&
			!fPlayerInput.isWPress)
		{
			IncreaseAttackCount(); //공격 카운트 증가
			MyPlayer->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
			MyPlayer->PlayAnimMontage(AirAttackMontage, attackSpeed, FName("AirAttack" + FString::FromInt(AttackCount)));
			MyPlayer->SetIsAttack(true);
			MyPlayer->SetIsMove(false);

		}
		else if (MyPlayer->GetIsAirPosition() &&
			!MyPlayer->GetIsATFAttack() &&
			fPlayerInput.isWPress)
		{
			MyPlayer->PlayAnimMontage(ATFAttack3Montage, attackSpeed, FName("AttackAirToFloorStart"));
			MyPlayer->SetIsAttack(true);
			MyPlayer->SetIsMove(false);
			MyPlayer->SetIsATFAttack(true);
		}
		else if (!MyPlayer->GetIsAirPosition())
		{
			IncreaseAttackCount(); //공격 카운트 증가
			MyPlayer->PlayAnimMontage(CurrentAttackMontage, attackSpeed, FName("Attack_1_" + FString::FromInt(AttackCount)));
			MyPlayer->SetIsAttack(true);
			MyPlayer->SetIsMove(false);

			//타게팅이 되어있을 경우 공격
			if (MyPlayer->GetIsLockOnEnemy())
			{
				if (fPlayerInput.isSPress && !MyPlayer->GetIsUpperAttack())
				{
					MyPlayer->SetIsUpperAttack(true);

					if (fPlayerInput.isLeftClickPressed)
					{
						MyPlayer->SetIsFloorToAirAttack(true);
						MyPlayer->PlayAnimMontage(UpperAttackFloorToJumpMontage, attackSpeed);
					}
					else
					{
						MyPlayer->PlayAnimMontage(UpperAttackMontage, attackSpeed);
					}

					MyPlayer->SetIsMove(false); //움직임을 막는다.
					MyPlayer->SetIsAttack(true); //공격중
					isLevitationAttack = true;//공중 판정
					ResetAttackCount(); //일반 공격 콤보중 실행 되었을 경우 공격 횟수를 0으로 초기화
				}

				if (fPlayerInput.isWPress && !MyPlayer->GetIsDashAttack())
				{
					ResetAttackCount();
					MyPlayer->SetIsDashAttack(true);
					MyPlayer->PlayAnimMontage(DashAttackMontage, attackSpeed);
					MyPlayer->SetIsMove(false); //움직임을 막는다.
					MyPlayer->SetIsAttack(true); //공격중
				}
			}
		}
	}
	else if (isEnableCombo)
	{
		isNextAttack = true;
	}
}

void ASwordWeaponActor::NextATFAttack()
{
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(GetOwner());
	MyPlayer->PlayAnimMontage(ATFAttack3Montage, attackSpeed, FName("AttackAirToFloorLoop"));
}

void ASwordWeaponActor::EndATFLoop()
{
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(GetOwner());
	MyPlayer->PlayAnimMontage(ATFAttack3Montage, attackSpeed, FName("AttackAirToFloorEnd"));
}

void ASwordWeaponActor::SubAttack()
{
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(GetOwner());
}

void ASwordWeaponActor::NextAttack()
{
	if (isNextAttack)
	{
		APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(GetOwner());
		IncreaseAttackCount();
		if (MyPlayer->GetIsAirPosition())
		{
			MyPlayer->PlayAnimMontage(AirAttackMontage, attackSpeed, FName("AirAttack" + FString::FromInt(AttackCount)));
		}
		else
		{
			MyPlayer->PlayAnimMontage(CurrentAttackMontage, attackSpeed, FName("Attack_1_" + FString::FromInt(AttackCount)));
		}

		isNextAttack = false;
	}
}


void ASwordWeaponActor::Dodge()
{
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(GetOwner());
	float OwnerDirection = MyPlayer->GetDirection();

	FName DodgeName;
	if (OwnerDirection < -90.f && OwnerDirection > -150.f)
	{
		DodgeName = "DodgeLeft";
	}

	if (OwnerDirection > 90.f && OwnerDirection < 150.f)
	{
		DodgeName = "DodgeRight";
	}


	MyPlayer->PlayAnimMontage(DodgeMontage, 1.2f, DodgeName);

	MyPlayer->SetIsMove(false);
	MyPlayer->SetIsDodge(true);
	MyPlayer->SetIsAttack(true);
}


void ASwordWeaponActor::SendDamage()
{
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(GetOwner());
	SetupDamagedTarget(MyPlayer);
	ApplyDamageToActor(MyPlayer);
}

void ASwordWeaponActor::KnockBackDamage()
{
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(GetOwner());
	MyPlayer->SetIsKnockBackAttack(true);//넉백공격
	SetupDamagedTarget(MyPlayer);
	ApplyDamageToActor(MyPlayer);
}

void ASwordWeaponActor::SetupDamagedTarget(class APlayerCharacter* MyPlayer)
{
	//데미지 전달 하기 전 초기화
	SendDamageToEnemyList.Empty();
	//데미지 전달
	FVector start, end, direction;
	start = MyPlayer->GetActorLocation();
	direction = MyPlayer->GetActorForwardVector();
	end = start + (direction * attackRange);

	//충돌 가능 오브젝드 유형
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	//오브젝트 타입 중 Pawn 채널을 PawnType변수에 넣어준다.
	TEnumAsByte<EObjectTypeQuery> PawnType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody);
	ObjectTypes.Add(PawnType);

	//무시할 액터 배열
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(MyPlayer); //충돌을 무시할 액터 배열에 자신(this)을 넣는다.

	isExecutedTrace = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), start, end, attackRadius, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, hitResults, true, FLinearColor::Red, FLinearColor::Green, 0.5f);

	if (isExecutedTrace)
	{
		//충돌한 액터에 Enemy태그가 있을 경우
		//중복을 제거하여 배열의 저장
		//블루프린트 Actor 카테고리에 Tags가 있음
		for (FHitResult hit : hitResults)
		{
			if (hit.Actor.Get()->ActorHasTag(FName(TEXT("Enemy"))))
			{
				ACommonCharacter* enemy = Cast<ACommonCharacter>(hit.GetActor());
				SendDamageToEnemyList.AddUnique(enemy);
				
				//피격된 에너미 객체에 피격 위치 벡터 설정
				enemy->SetHitLoc(hit.Location);

				if (MyPlayer->GetIsKnockBackAttack())
				{
					if (!enemy->ActorHasTag("Boss"))
					{

						enemy->KnockBackCharacter(MyPlayer->GetActorForwardVector());
						//enemy->LaunchCharacter(, false, false);
					}
				}
			}
		}
	}

}

void ASwordWeaponActor::ApplyDamageToActor(APlayerCharacter* MyPlayer)
{
	for (ACommonCharacter* damagedTarget : SendDamageToEnemyList)
	{
		//데미지 전달
		UGameplayStatics::ApplyDamage(damagedTarget, gi->GetDmgSword(), MyPlayer->GetController(), MyPlayer, UDamageType::StaticClass());
	}
}