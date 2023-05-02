// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h"

#include "PlayerCharacter.h"

#include "OrbsManagerComponent.h"


#include "OrbsProjectilesBaseActor.h"

// Sets default values
AOrbsProjectilesBaseActor::AOrbsProjectilesBaseActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));

	CollisionComponent->SetSphereRadius(16.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionProfileName("OrbsCollision");

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	//ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);

	ProjectileMovementComponent->bInitialVelocityInLocalSpace = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	OrbsParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("OrbsParticleSystem"));
	UParticleSystem* particleAsset;
	GetObjectAsset(particleAsset, UParticleSystem, "ParticleSystem'/Game/orbs_system/visual_data/particles/p_orb_projectile_short.p_orb_projectile_short'");
	OrbsParticleSystem->SetTemplate(particleAsset);
	OrbsParticleSystem->AttachTo(CollisionComponent);

	//Orbs컴포넌트 생성
	OrbsComponent = CreateDefaultSubobject<UOrbsManagerComponent>(TEXT("OrbsComponent"));

	//OrbsParticleHit
	GetObjectAsset(OrbsParticleHit, UParticleSystem, "ParticleSystem'/Game/orbs_system/visual_data/particles/p_orb_hit.p_orb_hit'");
}

// Called when the game starts or when spawned
void AOrbsProjectilesBaseActor::BeginPlay()
{
	Super::BeginPlay();
	MinFrameStartDelay = GetWorld()->GetDeltaSeconds() * MinFrameStartDelay;
	MaxFrameStartDelay = GetWorld()->GetDeltaSeconds() * MaxFrameStartDelay;
	float WaitTime = UKismetMathLibrary::RandomFloatInRange(MinFrameStartDelay, MaxFrameStartDelay); //시간을 설정하고
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			if (Type.IsNone())
			{
				InitOrbsData(InitialData);
			}
		}
	), WaitTime, false); //반복도 여기서 추가 변수를 선언해 설정가능
}

// Called every frame
void AOrbsProjectilesBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsStarted)
	{
		if (!IsFinished)
		{
			if (TargetActor)
			{
				UActorComponent* tmpComp = TargetActor->GetComponentByClass(UOrbsManagerComponent::StaticClass());
				if (tmpComp)
				{
					TargetComp = Cast<UOrbsManagerComponent>(tmpComp);
				}

				TickTargetLocation();
				TickAlpha();
			}

			TickVelocity();
			TickFinish();
		}
	}

	
	//LOG_SCREEN_T("GetActorLocation : X:%f, Y:%f, Z:%f", GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
	//LOG_SCREEN_T("TargetLocation : X:%f, Y:%f, Z:%f", TargetLocation.X, TargetLocation.Y, TargetLocation.Z);
}

void AOrbsProjectilesBaseActor::InitOrbsData(FOrbsReceiveData ReceiveData)
{
	InitialData = ReceiveData;
	SetOrbInit();
	float fInitOrbsDataExecuteTimer = UKismetMathLibrary::RandomFloatInRange(MinFrameStartDelay, MaxFrameStartDelay);

	GetWorld()->GetTimerManager().SetTimer(InitOrbsDataHandle, FTimerDelegate::CreateLambda([&]()
		{
			// 여기에 코드를 치면 된다.
			SetOrbStart();
			//FTimerHandle IsDetectedHandle;
			GetWorld()->GetTimerManager().SetTimer(IsDetectedHandle, FTimerDelegate::CreateLambda([&]()
				{
					// 여기에 코드를 치면 된다.
					IsDetect = true;
				}), DetectDelay, false);
	}), fInitOrbsDataExecuteTimer, false);
	
}

void AOrbsProjectilesBaseActor::SetOrbInit()
{
	SenderActor = InitialData.SenderActor;
	SpawnerActor = InitialData.SpawnerActor;
	TargetActor = InitialData.TargetActor;
	Type = InitialData.Type;
	Scale = InitialData.Scale;
	SendOptions = InitialData.SendOption;

	if (ScaleList.IsValidIndex((int32)Scale))
	{
		float FLocalScale = ScaleList[(int32)Scale] * ScaleMultiplyFactor;
		SetActorScale3D(FVector(FLocalScale));
	}

	bool LocalIsGetOrbs = false;
	int32 tmpIdx;
	FOrbsDynamicData LocalOrbsDData;
	OrbsComponent->GetOrbs(Type, LocalIsGetOrbs, tmpIdx, LocalOrbsDData);
	if (LocalIsGetOrbs)
	{
		OrbsColor = LocalOrbsDData.OrbsColor;
		OrbOptions = LocalOrbsDData.OrbsOption;
	}

	OrbsParticleSystem->SetColorParameter("color", OrbsColor);

	if (SenderActor)
	{
		InitialVelocity = SenderActor->GetVelocity();
	}
	
	FVector LocalProjectileDir = UKismetMathLibrary::SelectVector(
		InitialVelocity.GetSafeNormal(),
		GetOwner()->GetActorForwardVector(),
		(InitialVelocity.GetSafeNormal() != FVector(0,0,0))
		);

	float LocalProjectileVelocity = UKismetMathLibrary::SelectFloat(
		InitialVelocity.Size(),
		StartInitialVelocity,
		(InitialVelocity.Size() > StartInitialVelocity)
	);

	float LocalProjectileVelocityClamp = UKismetMathLibrary::FClamp(LocalProjectileVelocity, 1.0f, LocalProjectileVelocity);

	ProjectileMovementComponent->Velocity = LocalProjectileDir * LocalProjectileVelocityClamp;

	//use rotation mode by var
	ProjectileMovementComponent->bRotationFollowsVelocity = !bRotateByManualInterp;

	//we select random direction of 360 of yaw, and up to 90 of pitch.
	FRotator LocalProjectilesRot = FRotator(
		UKismetMathLibrary::RandomFloatInRange(InitialPitchRange.X, InitialPitchRange.Y),
		UKismetMathLibrary::RandomFloatInRange(InitialYawRange.X, InitialYawRange.Y),
		0);


	StartingVelocity = LocalProjectilesRot.Vector().GetSafeNormal();

	//set the current velocity
	ProjectileMovementComponent->Velocity = StartingVelocity * UKismetMathLibrary::FClamp(
		ProjectileMovementComponent->Velocity.Size(),
		1.0f,
		ProjectileMovementComponent->Velocity.Size()
	);
}

void AOrbsProjectilesBaseActor::SetOrbStart()
{
	//pick random initial launch speed.
	InitialSpeed = UKismetMathLibrary::RandomFloatInRange(MinInitialSpeed, MaxInitialSpeed);

	//pick random target speed, this will be the constant speed of flying.
	TargetSpeed = UKismetMathLibrary::RandomFloatInRange(MinTargetSpeed, MaxTargetSpeed);

	//random delay value before starting detecting target.
	DetectDelay = UKismetMathLibrary::RandomFloatInRange(MinDetectTargetSpeed, MaxDetectTargetSpeed);

	//random alpha speed value,	the speed of detecting target.
	DetectInterp = UKismetMathLibrary::RandomFloatInRange(MinDetectTargetInterpSpeed, MaxDetectTargetInterpSpeed);

	//random interp value, target velocity interpolation speed.
	TargetInterp = UKismetMathLibrary::RandomFloatInRange(MinVelocityTargetInterpSpeed, MaxVelocityTargetInterpSpeed);

	//set started bool to process tick logic.
	IsStarted = true;


}

void AOrbsProjectilesBaseActor::TickTargetLocation()
{
	if (TargetActor)
	{ 
		TargetLocation = TargetActor->GetRootComponent()->GetComponentVelocity();

		if (TargetComp)
		{
			//USceneComponent* test = TargetComp->GetSourceMeshObject();
			//if (test)
			//{
			//	TargetLocation = Cast<UPrimitiveComponent>(TargetComp->GetSourceMeshObject())->GetSocketLocation(TargetComp->GetSourceMeshSocket());
			//}
			//else
			//{
			//}
			TargetLocation = TargetActor->GetActorLocation();


		}
		else
		{
			TargetLocation = TargetActor->GetActorLocation();
		}
	}
	else
	{
		TargetLocation = GetActorLocation() + ProjectileMovementComponent->Velocity;
	}


}

void AOrbsProjectilesBaseActor::TickAlpha()
{
	if (bEnableTargetPullMode)
	{
		if (TargetComp)
		{
			FVector targetToLength = GetActorLocation() - TargetLocation;

			IsDetect = (targetToLength.Size() < TargetPullRadius && TargetComp->GetEnableOrbsPull());
		}
		else
		{
			IsDetect = false;
		}
	}
	else
	{
		if (bEnableTargetPullRadius)
		{
			FVector targetToLength = GetActorLocation() - TargetLocation;
			IsDetect = (targetToLength.Size() < TargetPullRadius && TargetComp->GetEnableOrbsPull());
		}
	}
	
	TargetAlpha = UKismetMathLibrary::FClamp(
		UKismetMathLibrary::FInterpTo(TargetAlpha,
			UKismetMathLibrary::SelectFloat(1.05f, -0.05f, IsDetect),
			GetWorld()->GetDeltaSeconds(),
			DetectInterp)
		, 0.0f, 1.0f);
}

void AOrbsProjectilesBaseActor::TickVelocity()
{
	float DeltaSec = GetWorld()->GetDeltaSeconds();
	//flying towards direction scale, from 4 to 14 second of game time since this orb creation, it will increase flying speed towards direction of target.
	//GetGameTimeSinceCreation()

	float LocalTargetSpeedModifier;

	float PowerRange = UKismetMathLibrary::MapRangeClamped(GetGameTimeSinceCreation(), MinTargetTime, MaxTargetTime, MinTargetSpeedOverTimePower, MaxTargetSpeedOverTimePower);


	float MinRange = UKismetMathLibrary::MapRangeClamped(TargetVelocity.Size(), 0.0f, 1000.0f, 1.0f, MinTargetMovingSpeedPower);

	float MaxRange = UKismetMathLibrary::MapRangeClamped(TargetVelocity.Size(), 1000.0f, 10000.0f, 1.0f, MaxTargetMovingSpeedPower);

	//add additional boost by target velocity object, so if target moves faster we will make orb move faster too to reach target.
	float VelocityTmp = UKismetMathLibrary::SelectFloat(MinRange, MaxRange, (MinRange < 1000.f));

	LocalTargetSpeedModifier = (PowerRange + VelocityTmp) * TargetSpeed;

	FVector LocalTargetVelocity = UKismetMathLibrary::GetDirectionUnitVector(GetOwner()->GetActorLocation(), TargetLocation).GetSafeNormal() * LocalTargetSpeedModifier;

	//chaotic random rotation directions, orbs will blend by flying up / down and etc.
	FRotator ChaoticRot = FRotator(UKismetMathLibrary::RandomFloatInRange(-70.0f, 70.0f),
		UKismetMathLibrary::RandomFloatInRange(-35.0f, 35.0f),
		UKismetMathLibrary::RandomFloatInRange(-49.0f, 49.0f));

	//chaotic movements scale, from 4 to 14 seconds of game time since this orb creation 	it will increase its velocity power of chaotic movements.

	//GetGameTimeSinceCreation()
	float LocalChaoticSpeedTime = UKismetMathLibrary::MapRangeClamped(GetGameTimeSinceCreation(), MinChaoticTime, MaxChaoticTime, MinChaoticSpeedOverTime, MaxChaoticSpeedOverTime);

	float LocalChaoticPowerOvertime = UKismetMathLibrary::MapRangeClamped(GetGameTimeSinceCreation(), MinChaoticTime, MaxChaoticTime, MinChaoticPowerOverTime, MaxChaoticPowerOverTime);

	FRotator CombineRotChaotic = LocalTargetVelocity.GetSafeNormal().Rotation() + (ChaoticRot * LocalChaoticPowerOvertime);

	FVector LocalChaoticVelocity = UKismetMathLibrary::GetForwardVector(CombineRotChaotic).GetSafeNormal() * LocalChaoticSpeedTime;

	//if target agent is close enough, we will use speed limit of 10, 000, if target moves faster limit will be increased by 25, 000 this will make orbs speed more stable.
	float TargetToActorLength = (GetOwner()->GetActorLocation() - TargetLocation).Size();

	//we will increase speed limit 	by target velocity value, so if target moves faster orb speed limit is higher.
	float TargetVelocityLength = TargetVelocity.Size();
	float TargetVelocityLengthMapClampedA = UKismetMathLibrary::MapRangeClamped(TargetVelocityLength, 0.0f, 1000.0f, 500.0f, 2000.0f);

	float TargetVelocityLengthMapClampedB = UKismetMathLibrary::MapRangeClamped(TargetVelocityLength, 1000.0f, 10000.0f, 2000.0f, 15000.0f);

	float LocalMaxSpeedModifierOutRangeA = UKismetMathLibrary::SelectFloat(TargetVelocityLengthMapClampedA, TargetVelocityLengthMapClampedB, (TargetVelocityLength < 1000.f));

	float LocalMaxSpeedModifierOutRangeB = UKismetMathLibrary::MapRangeClamped(TargetVelocityLength, 1000.f, 20000.f, 10000.f, 25000.f);

	float LocalMaxSpeedModifier = UKismetMathLibrary::MapRangeClamped(TargetToActorLength, 100.f, 400.f, LocalMaxSpeedModifierOutRangeA, LocalMaxSpeedModifierOutRangeB);

	FVector ProjectileVelocity;
	//current flying speed and velocity, flying forward direction. : float InitialSpeed
	FVector ProjectileInitSpeed = (ProjectileMovementComponent->Velocity.GetSafeNormal() * InitialSpeed);
	FVector SelectedChaoticSpeed = UKismetMathLibrary::SelectVector(LocalChaoticVelocity, FVector(0, 0, 0), bEnableChaoticWithoutTarget);

	FVector VLerpVelocity = UKismetMathLibrary::VLerp(ProjectileInitSpeed + SelectedChaoticSpeed, LocalTargetVelocity + LocalChaoticVelocity, TargetAlpha);

	//clamp adjusted vector size as target.
	FVector LocalProjectileVelocity = UKismetMathLibrary::ClampVectorSize(VLerpVelocity, 0.f, LocalMaxSpeedModifier);

	FVector SetProjectilVelA = UKismetMathLibrary::VInterpTo(ProjectileMovementComponent->Velocity, LocalProjectileVelocity, DeltaSec, TargetInterp);

	FVector SetProjectileVelocity = UKismetMathLibrary::ClampVectorSize(SetProjectilVelA, 0.0f, LocalMaxSpeedModifier);

	//set to projectile as final velocity.
	ProjectileMovementComponent->Velocity = SetProjectileVelocity;

	if (bRotateByManualInterp)
	{
		//skip using interpolated velocity for rotation and use future velocity, to make rotations with better look, otherwise it will look delayed
		FRotator SetProjectileRot = UKismetMathLibrary::RInterpTo(GetActorRotation(), LocalProjectileVelocity.GetSafeNormal().Rotation(), DeltaSec, RotateManualInterpSpeed);

		SetActorRotation(SetProjectileRot);

	}

}

void AOrbsProjectilesBaseActor::TickFinish()
{

	if (TargetActor)
	{
		//LOG_SCREEN_T("TickFinish::TargetAlpha : %f", TargetAlpha);
		//check if target alpha is at least half before finish
		if (TargetAlpha >= 0.5f || (TargetAlpha < 0.5f && bEnableTargetPullMagnet))
		{
			float TargetToProjectileDist = FVector::Dist(GetActorLocation(), TargetLocation);
			//LOG_SCREEN_T("TargetToProjectileDist : %f", TargetToProjectileDist);
			if (TargetToProjectileDist < MinDistanceToFinishDelivery)
			{
				if (!IsFinished)
				{
					IsFinished = true;
					SetActorTickEnabled(false);
					FinishReceive();
				}
			}
			else
			{
				if (TargetToProjectileDist < MinDistanceToMagnetDelivery)
				{
					LocalVlerpRate += GetWorld()->GetDeltaSeconds();
					SetActorLocation(
						UKismetMathLibrary::VLerp(GetActorLocation(), TargetLocation, LocalVlerpRate)
					);
				}
			}
		}

		if (GetGameTimeSinceCreation() >= MaxLifespanWithTarget)
		{
			FinishOrbs();
		}

	}
	else
	{
		if (GetGameTimeSinceCreation() >= MaxLifespanNoTarget)
		{
			FinishOrbs();
		}
	}
}

void AOrbsProjectilesBaseActor::FinishReceive()
{
	if (TargetActor)
	{
		if (TargetComp)
		{
			TargetComp->SetOrbs(Type, 1, Scale, true, false);
			FOrbsReceiveData rData;
			rData.SenderActor = SenderActor;
			rData.SpawnerActor = SpawnerActor;
			rData.OrbActor = this;
			rData.TargetActor = TargetActor;
			rData.Type = Type;
			rData.Scale = Scale;
			rData.SendOption = SendOptions;
			TargetComp->EdOrbReceiveSingle.Broadcast(rData);
			
			TargetComp->OrbsRefreshWidget();
			FinishOrbs();
		}
		else
		{
			FinishOrbs();
		}
	}
	else
	{
		FinishOrbs();
	}
}

void AOrbsProjectilesBaseActor::FinishOrbs()
{
	OrbsParticleSystem->Deactivate();
	ProjectileMovementComponent->Deactivate();
	
	float DeltaSec = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	float LocalFinishDuration = UKismetMathLibrary::RandomFloatInRange(DeltaSec * 4.0f, DeltaSec * 8.0f);
	GetWorld()->GetTimerManager().SetTimer(FinishTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			//if target actor is valid, spawn hit emitter attached to it.
			FVector SpawnHitParticleScale = UKismetMathLibrary::Conv_FloatToVector(UKismetMathLibrary::RandomFloatInRange(0.7f, 14.0f));

			if (TargetActor)
			{
				TSet<UActorComponent*> particles = TargetActor->GetComponents();
				if (particles.Num() < HitParticlesMaxAttachOptimization)
				{

					UParticleSystemComponent* hitPsComp = UGameplayStatics::SpawnEmitterAttached(OrbsParticleHit, TargetActor->GetRootComponent(), "None", TargetLocation, FRotator(0, 0, 0), SpawnHitParticleScale, EAttachLocation::KeepWorldPosition, true, EPSCPoolMethod::None, true);

					hitPsComp->SetColorParameter("color", OrbsColor);

				}
			}
			else
			{
				UParticleSystemComponent* hitPsComp = UGameplayStatics::SpawnEmitterAttached(OrbsParticleHit, CollisionComponent, "None", GetOwner()->GetActorLocation(), FRotator(0, 0, 0), SpawnHitParticleScale, EAttachLocation::KeepWorldPosition, true, EPSCPoolMethod::None, true);

				hitPsComp->SetColorParameter("color", OrbsColor);
			}

			GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, FTimerDelegate::CreateLambda([&]()
				{

					Destroy();
				}), UKismetMathLibrary::RandomFloatInRange(0.3f, 0.7f), false);

		}), LocalFinishDuration, false);

}

