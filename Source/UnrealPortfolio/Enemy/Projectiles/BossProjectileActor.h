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

	//Ȱ��ȭ, ��Ȱ��ȭ �Լ�
	void SetActive(bool InActive);

	//��Ȱ��ȭ �Լ�
	void Deactivator();

	//Ȱ��ȭ ��Ȱ��ȭ ���� ��ȯ �Լ�.
	bool IsActive();

	void FireInDirection(const FVector& direction);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bActive; //Ȱ��ȭ ��Ȱ��ȭ ���� ����

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
