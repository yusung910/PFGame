// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossProjectileActor.generated.h"

UCLASS()
class UNREALPORTFOLIO_API ABossProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossProjectileActor();

	//활성화, 비활성화 함수
	void SetActive(bool InActive);

	//비활성화 함수
	void Deactivator();

	//활성화 비활성화 여부 반환 함수.
	bool IsActive();

	void FireInDirection(const FVector& direction);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bActive; //활성화 비활성화 여부 변수

protected:
	UPROPERTY(VisibleAnywhere)
		class USphereComponent* CollisionComponent;
	UPROPERTY(VisibleAnywhere)
		class UParticleSystemComponent* SpellParticleSystem;

	//UPROPERTY(VisibleAnywhere)
	//	class UParticleSystem* SpellHitParticle;
	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* ProjectileMovementComponent;

	FTimerHandle TimerHandle;

	float SpellRadius = 150.f;

	float Damage = 15.f;

	UFUNCTION()
		void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
