// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavigationLaserActor.generated.h"

UCLASS()
class UNREALPORTFOLIO_API ANavigationLaserActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANavigationLaserActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetLaserPointDirection(FVector s, FVector e);

private:
	UPROPERTY()
		class UStaticMeshComponent* MyMesh;

	UPROPERTY(EditAnywhere, Category = "LaserParticle", meta = (AllowPrivateAccess = true))
		class UParticleSystem* NaviLaserParticle;

	UPROPERTY(EditAnywhere, Category = "LaserParticle", meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* NaviLaserParticleComp;
};
