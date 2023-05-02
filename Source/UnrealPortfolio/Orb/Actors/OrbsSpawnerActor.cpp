// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"
#include "OrbStructActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h"

#include "OrbsManagerComponent.h"
#include "OrbsSpawnerActor.h"
#include "OrbsProjectilesBaseActor.h"

// Sets default values
AOrbsSpawnerActor::AOrbsSpawnerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//ProjectileClass = Cast<AOrbsProjectilesBaseActor>(AOrbsProjectilesBaseActor::StaticClass()->GetDefaultObject());
}

// Auto destroy by beginplay. Because spawner should be not placed in world. But spawned by sender omponent.
void AOrbsSpawnerActor::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle WaitHandle;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			//LOG_SCREEN("NeedToSpawnAmount : %d", NeedToSpawnAmount);
			if (NeedToSpawnAmount <= 0)
			{
				DestroySpawner();
			}

		}), 0.2f, false); //반복도 여기서 추가 변수를 선언해 설정가능
}

// Called every frame
void AOrbsSpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//LOG_SCREEN_T("NeedToSpawnAmount : %d", NeedToSpawnAmount);
	if (bExecuteSpawnOrbs && !bIsDestroySpawner)
	{
		fSpawnOrbsTimeCnt += DeltaTime;
		if (fSpawnOrbsTimer <= fSpawnOrbsTimeCnt)
		{

			bExecuteSpawnOrbs = false;
			RegenerateSpawnOrbs();
		}
	}
}


void AOrbsSpawnerActor::SpawnOrbs(AActor* sender, UOrbsManagerComponent* comp, FOrbsSendData sData)
{
	SenderActor = sender;
	SenderComponent = comp;
	SendData = sData;

	if (SenderComponent)
	{
		NeedToSpawnAmount = SendData.Amount;

		if (UKismetSystemLibrary::IsValidClass(SendData.ProjectilesClass->StaticClass()))
		{
			ProjectileClass = SendData.ProjectilesClass;
		}
		else
		{
			ProjectileClass = SenderComponent->GetDefaultProjectilesClass();
		}

		RegenerateSpawnOrbs();
	}
	else
	{
		DestroySpawner();
	}

}

void AOrbsSpawnerActor::RegenerateSpawnOrbs()
{
	float DeltaSec = GetWorld()->GetDeltaSeconds();
	if (bUseLoopsOptimization)
	{
		//check if currently spawned amount by spawner is less than max allowable local spawns.
		if (SpawnerOrbsList.Num() < LocalMaxOrbsOptimization)
		{
			TArray<AActor*> AllProjectilesInWorld;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOrbsProjectilesBaseActor::StaticClass(), AllProjectilesInWorld);

			
			if (AllProjectilesInWorld.Num() < WorldMaxOrbsOptimization)
			{
				//LOG_SCREEN("WorldMaxOrbsOptimization : %d, AllProjectilesInWorld.Num() : %d", WorldMaxOrbsOptimization, AllProjectilesInWorld.Num());

				for (int i = 0; i < OrbsPerLoopOptimization - 1; i++)
				{
					if (NeedToSpawnAmount > 0)
					{
						SpawnOrbsActor();
					}
					else
					{
						DestroySpawner();
					}

				}
				if (NeedToSpawnAmount > 0)
				{
					//loop 실행
					fSpawnOrbsTimer = UKismetMathLibrary::RandomFloatInRange(MinSpawnerFrameDelay * DeltaSec, MaxSpawnerFrameDelay * DeltaSec);
					bExecuteSpawnOrbs = true;
				}
				else
				{
					DestroySpawner();
				}
			}
			else
			{
				//loop 실행
				fSpawnOrbsTimer = UKismetMathLibrary::RandomFloatInRange(MinSpawnerFrameDelay * DeltaSec, MaxSpawnerFrameDelay * DeltaSec);
				bExecuteSpawnOrbs = true;
				fSpawnOrbsTimeCnt = 0.0f;
			}
		}
		else
		{
			//loop 실행
			fSpawnOrbsTimer = UKismetMathLibrary::RandomFloatInRange(MinSpawnerFrameDelay * DeltaSec, MaxSpawnerFrameDelay * DeltaSec);
			bExecuteSpawnOrbs = true;
			fSpawnOrbsTimeCnt = 0.0f;
		}
	}
	else
	{
		//directly check if there are amount of orbs need to be spawned.
		if (NeedToSpawnAmount > 0)
		{
			SpawnOrbsActor();
			//loop 실행
			fSpawnOrbsTimer = UKismetMathLibrary::RandomFloatInRange(MinSpawnerFrameDelay, MaxSpawnerFrameDelay);
			bExecuteSpawnOrbs = true;
			fSpawnOrbsTimeCnt = 0.0f;
		}
		else
		{
			DestroySpawner();
		}
	}
}

//
void AOrbsSpawnerActor::SpawnOrbsActor()
{
	NeedToSpawnAmount -= 1;
	//spawn orbs spawner at location null.
	FActorSpawnParameters spawnParam;
	spawnParam.Owner = GetOwner();
	FTransform LocalActorTransform = GetOwner()->GetActorTransform();

	LocalActorTransform.SetLocation(GetOwner()->GetActorLocation());

	AOrbsProjectilesBaseActor* tmpProjectiles = GetWorld()->SpawnActor<AOrbsProjectilesBaseActor>(ProjectileClass->StaticClass(), LocalActorTransform, spawnParam);

	SpawnerOrbsList.Add(tmpProjectiles);

	FOrbsReceiveData rData;

	rData.SenderActor = SenderActor;
	rData.SpawnerActor = this;
	rData.TargetActor = SendData.TargetActor;
	rData.Type = SendData.Type;
	rData.Scale = SendData.Scale;
	rData.SendOption = SendData.SendOption;
	rData.Amount = 1;

	tmpProjectiles->InitOrbsData(rData);
	if (SenderActor)
	{
		UOrbsManagerComponent* orbsActorComp = Cast<UOrbsManagerComponent>(SenderActor->GetComponentByClass(UOrbsManagerComponent::StaticClass()));
		//get sender actor's manager to notify each sending.
		if (orbsActorComp)
		{
			//if dynamic decrease, then decrease amount of sender orbs each time orb spawns. so if player sends 100 orbs, the existing amount will be decreased dynamically.
			if (SendData.bDynamicDecrease)
			{
				orbsActorComp->SetOrbs(SendData.Type, -1, SendData.Scale, true, false);
			}
			orbsActorComp->OrbsRefreshWidget();

			//call dispatch notify on sender, for single orb sending started.
			orbsActorComp->EdOrbSendSingle.Broadcast(SendData);
		}
	}

	
}

void AOrbsSpawnerActor::DestroySpawner()
{
	bIsDestroySpawner = true;
	if (SenderComponent)
	{
		SenderComponent->RemoveSpawner(this);
	}

	K2_DestroyActor();
}



