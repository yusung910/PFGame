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

	//�ʱ� ���� ��Ÿ�� ����
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
	//���� �õ�
	if (!MyPlayer->GetIsAttack())
	{
		if (MyPlayer->GetIsAirPosition() &&
			!fPlayerInput.isSPress &&
			!fPlayerInput.isWPress)
		{
			IncreaseAttackCount(); //���� ī��Ʈ ����
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
			IncreaseAttackCount(); //���� ī��Ʈ ����
			MyPlayer->PlayAnimMontage(CurrentAttackMontage, attackSpeed, FName("Attack_1_" + FString::FromInt(AttackCount)));
			MyPlayer->SetIsAttack(true);
			MyPlayer->SetIsMove(false);

			//Ÿ������ �Ǿ����� ��� ����
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

					MyPlayer->SetIsMove(false); //�������� ���´�.
					MyPlayer->SetIsAttack(true); //������
					isLevitationAttack = true;//���� ����
					ResetAttackCount(); //�Ϲ� ���� �޺��� ���� �Ǿ��� ��� ���� Ƚ���� 0���� �ʱ�ȭ
				}

				if (fPlayerInput.isWPress && !MyPlayer->GetIsDashAttack())
				{
					ResetAttackCount();
					MyPlayer->SetIsDashAttack(true);
					MyPlayer->PlayAnimMontage(DashAttackMontage, attackSpeed);
					MyPlayer->SetIsMove(false); //�������� ���´�.
					MyPlayer->SetIsAttack(true); //������
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
	MyPlayer->SetIsKnockBackAttack(true);//�˹����
	SetupDamagedTarget(MyPlayer);
	ApplyDamageToActor(MyPlayer);
}

void ASwordWeaponActor::SetupDamagedTarget(class APlayerCharacter* MyPlayer)
{
	//������ ���� �ϱ� �� �ʱ�ȭ
	SendDamageToEnemyList.Empty();
	//������ ����
	FVector start, end, direction;
	start = MyPlayer->GetActorLocation();
	direction = MyPlayer->GetActorForwardVector();
	end = start + (direction * attackRange);

	//�浹 ���� �������� ����
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	//������Ʈ Ÿ�� �� Pawn ä���� PawnType������ �־��ش�.
	TEnumAsByte<EObjectTypeQuery> PawnType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody);
	ObjectTypes.Add(PawnType);

	//������ ���� �迭
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(MyPlayer); //�浹�� ������ ���� �迭�� �ڽ�(this)�� �ִ´�.

	isExecutedTrace = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), start, end, attackRadius, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, hitResults, true, FLinearColor::Red, FLinearColor::Green, 0.5f);

	if (isExecutedTrace)
	{
		//�浹�� ���Ϳ� Enemy�±װ� ���� ���
		//�ߺ��� �����Ͽ� �迭�� ����
		//�������Ʈ Actor ī�װ��� Tags�� ����
		for (FHitResult hit : hitResults)
		{
			if (hit.Actor.Get()->ActorHasTag(FName(TEXT("Enemy"))))
			{
				ACommonCharacter* enemy = Cast<ACommonCharacter>(hit.GetActor());
				SendDamageToEnemyList.AddUnique(enemy);
				
				//�ǰݵ� ���ʹ� ��ü�� �ǰ� ��ġ ���� ����
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
		//������ ����
		UGameplayStatics::ApplyDamage(damagedTarget, gi->GetDmgSword(), MyPlayer->GetController(), MyPlayer, UDamageType::StaticClass());
	}
}