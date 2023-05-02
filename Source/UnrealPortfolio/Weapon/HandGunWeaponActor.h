// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OrbStructActor.h"

#include "CoreMinimal.h"
#include "WeaponActor.h"
#include "HandGunWeaponActor.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHandGunWeaponActor : public AWeaponActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	AHandGunWeaponActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Particle")
		class UParticleSystem* HandGunFireParticle;

	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* ShootFromLand;

	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* ShootFromAir;

	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* HandGunDodge;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Attack() override;

	virtual void SendDamage() override;

	virtual void Dodge() override;

	UFUNCTION()
		void GunFire(APlayerCharacter* MyPlayer);
private:
	float JumpZVal = 2000.f;
	
};
