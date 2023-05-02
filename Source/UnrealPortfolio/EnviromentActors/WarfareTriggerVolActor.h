// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarfareTriggerVolActor.generated.h"

UCLASS()
class UNREALPORTFOLIO_API AWarfareTriggerVolActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWarfareTriggerVolActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
		class UBoxComponent* TriggerZone;

	UFUNCTION()
		virtual void NotifyActorBeginOverlap(AActor* OtherActor);

	UFUNCTION()
		virtual void NotifyActorEndOverlap(AActor* OtherActor);

private:
	UPROPERTY()
		class UMainGameInstance* gi;

	UPROPERTY()
		class AMainGameMode* MainGM;
	


	UPROPERTY(EditAnywhere, Category = "LaserParticle", meta = (AllowPrivateAccess = true))
		class UParticleSystem* WarpParticle;

	UPROPERTY(EditAnywhere, Category = "LaserParticle", meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* WarpParticleComp;
};
