// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OrbStructActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OrbsSpawnerActor.generated.h"

UCLASS()
class UNREALPORTFOLIO_API AOrbsSpawnerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOrbsSpawnerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SpawnOrbs(AActor* SenderActor, UOrbsManagerComponent* comp, FOrbsSendData sData);

	UFUNCTION()
		void RegenerateSpawnOrbs();

	UFUNCTION()
		void SpawnOrbsActor();

	UFUNCTION()
		void DestroySpawner();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		class AActor* SenderActor;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		class UOrbsManagerComponent* SenderComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		FOrbsSendData SendData;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		class AOrbsProjectilesBaseActor* ProjectileClass;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		TArray<AOrbsProjectilesBaseActor*> SpawnerOrbsList;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		int32 NeedToSpawnAmount = 0;
	 
private:
	//use little loops for performance boost or fully spawn all amount.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Spawner Optimization")
		bool bUseLoopsOptimization = true;

	//Local max orbs limit for this spawner, all the orbs bypass this limit will wait in queue before other orbs will be finished before spawning next ones.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Spawner Optimization")
		int32 LocalMaxOrbsOptimization = 1000;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Spawner Optimization")
		int32 WorldMaxOrbsOptimization = 500;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Spawner Optimization")
		int32 OrbsPerLoopOptimization = 7;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Spawner Optimization")
		float MinSpawnerFrameDelay = 10.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Spawner Optimization")
		float MaxSpawnerFrameDelay = 20.0f;

private:
	UPROPERTY()
		bool bExecuteSpawnOrbs = false;
	UPROPERTY()
		bool bIsDestroySpawner = false;

	UPROPERTY()
		float fSpawnOrbsTimer = 0.0f;
	UPROPERTY()
		float fSpawnOrbsTimeCnt = 0.0f;
		
};
