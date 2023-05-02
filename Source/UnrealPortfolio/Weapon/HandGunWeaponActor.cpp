// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "MainPlayerController.h"

#include "DrawDebugHelpers.h"

#include "OrbsManagerComponent.h"
#include "Components/CapsuleComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Particles/ParticleSystemComponent.h"

#include "MainGameInstance.h"

#include "HandGunWeaponActor.h"

AHandGunWeaponActor::AHandGunWeaponActor()
{
	UStaticMesh* MeshAsset;
	GetObjectAsset(MeshAsset, UStaticMesh, "StaticMesh'/Game/TPS_HandGun/TPS_HandGun/Weapon/Mesh/SM_Revolver.SM_Revolver'");  

	MyMesh->SetStaticMesh(MeshAsset);
	MyMesh->AttachTo(Root);

	GetObjectAsset(ShootFromLand, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Player/HandGun_Shoot_Floor_Montage.HandGun_Shoot_Floor_Montage'");

	GetObjectAsset(ShootFromAir, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Player/HandGun_Aim_Jump_Shoot_Montage.HandGun_Aim_Jump_Shoot_Montage'");

	GetObjectAsset(HandGunDodge, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Player/HandGun_Dodge_montage.HandGun_Dodge_montage'");

	//권총 관련 파이어 파티클 HandGunFireParticle
	GetObjectAsset(HandGunFireParticle, UParticleSystem, "ParticleSystem'/Game/ParagonRevenant/FX/Particles/Revenant/Abilities/Primary/FX/P_Revenant_LastShot_MuzzleFlash_Locked.P_Revenant_LastShot_MuzzleFlash_Locked'");
	
}

void AHandGunWeaponActor::BeginPlay()
{
	Super::BeginPlay();
}

void AHandGunWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(GetOwner());
	if (!MyPlayer->GetIsAirPosition())
	{
		JumpZVal = 500.f;
	}
	
}

void AHandGunWeaponActor::Attack()
{
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(GetOwner());

	if (!MyPlayer->GetIsLockOnEnemy())
		return;

	if (!MyPlayer->GetIsAttack())
	{
		MyPlayer->SetIsMove(true);
		MyPlayer->SetIsAttack(true);
		MyPlayer->SetIsDashAttack(true);
		MyPlayer->SetIsFloorToAirAttack(true);
		MyPlayer->SetIsbeingGrapped(true);
		GunFire(MyPlayer);
	}
}

void AHandGunWeaponActor::GunFire(APlayerCharacter* MyPlayer)
{
	float LockOnPitch = MyPlayer->GetLookAtPitch();

	FName sectionNm = "GunShotFront";

	if (LockOnPitch < 0.f)
	{
		sectionNm = "GunShotDown";
	}
	else if (LockOnPitch > 0.f)
	{
		sectionNm = "GunShotUp";
	}

	if (MyPlayer->GetIsAir())
	{
		MyPlayer->PlayAnimMontage(ShootFromAir, 2.0f, sectionNm);
		FVector jumpLoc = MyPlayer->GetActorLocation();
		jumpLoc.Z = JumpZVal;
		MyPlayer->LaunchCharacter(jumpLoc, false, false);
		JumpZVal -= 100.f;
	}
	else
	{
		MyPlayer->PlayAnimMontage(ShootFromLand, 2.0f, sectionNm);
	}

	FVector PosParticle = AddFVectorX(GetActorLocation(), 20.f);

	MyPlayer->SetIsAttack(true);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HandGunFireParticle, PosParticle);
}


void AHandGunWeaponActor::SendDamage()
{
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(GetOwner());
	if (MyPlayer->GetIsLockOnEnemy())
	{
		//데미지 전달
		FVector start, end;
		start = MyPlayer->GetActorLocation();
		end = MyPlayer->GetLockEnemy()->GetActorLocation();

		//충돌 가능 오브젝트 유형
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

		//오브젝트 타입 중 Pawn 채널을 PawnType변수에 넣어준다.
		TEnumAsByte<EObjectTypeQuery> PawnType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody);
		ObjectTypes.Add(PawnType);

		//무시할 액터 배열
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(MyPlayer); //충돌을 무시할 액터 배열에 자신(this)을 넣는다.

		//충돌 결과
		FHitResult hitResult;

		if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), start, end, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, hitResult, true, FLinearColor::Red, FLinearColor::Green, 0.5f))
		{
			UMainGameInstance* gi = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
			UGameplayStatics::ApplyDamage(hitResult.GetActor(), gi->GetDmgGun(), MyPlayer->GetController(), MyPlayer, UDamageType::StaticClass());

			if (hitResult.GetActor())
			{
				//피격된 에너미 객체에 이펙트 재생
				Cast<ACommonCharacter>(hitResult.GetActor())->SetHitLoc(hitResult.Location);
			}
		}
	}
}

void AHandGunWeaponActor::Dodge()
{
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(GetOwner());
	float OwnerDirection = MyPlayer->GetDirection();
	FName DodgeName;
	if (OwnerDirection < -90.f && OwnerDirection > -150.f)
	{
		DodgeName = "HandGunDodgeLeft";
	}

	if (OwnerDirection > 90.f && OwnerDirection < 150.f)
	{
		DodgeName = "HandGunDodgeRight";
	}


	MyPlayer->PlayAnimMontage(HandGunDodge, 1.2f, DodgeName);

	MyPlayer->SetIsMove(false);
	MyPlayer->SetIsDodge(true);
	MyPlayer->SetIsAttack(true);
}

