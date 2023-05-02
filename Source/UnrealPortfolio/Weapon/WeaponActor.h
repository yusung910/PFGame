// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPlayerControllerInputFlag.h"

#include "WeaponActor.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AWeaponActor : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;

	UPROPERTY()
		class UStaticMeshComponent* MyMesh;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
		int32 AttackCount = 0;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
		bool isNextAttack = false;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
		bool isEnableCombo = false;

	UPROPERTY()
		FPlayerControllerInputFlag fPlayerInput;

	UPROPERTY()
		bool isLevitationAttack = false;

public:

	AWeaponActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Attack() {};

	virtual void Dodge() {};

	virtual void SubAttack() {};

	virtual void NextAttack() {};

	virtual void UpperAttack() {};

	virtual void NextATFAttack() {};

	virtual void EndATFLoop() {};

	virtual void SendDamage() {};

	virtual void KnockBackDamage() {};
	

	UFUNCTION()
		bool SendDamage(AActor* DamagedActor, float dmg);

	FORCEINLINE void IncreaseAttackCount() { AttackCount++; }
	FORCEINLINE int32 GetAttackCount() { return AttackCount; }
	FORCEINLINE void ResetAttackCount() { AttackCount = 0; }

	FORCEINLINE void SetIsNextAttack(bool val) { isNextAttack = val; }
	FORCEINLINE bool GetIsNextAttack() { return isNextAttack; }

	FORCEINLINE bool GetIsEnableCombo() { return isEnableCombo; }
	FORCEINLINE void SetIsEnableCombo(bool value) { isEnableCombo = value; }

	FORCEINLINE bool GetIsLevitationAttack() { return isLevitationAttack; }
	FORCEINLINE void SetIsLevitationAttack(bool value) { isLevitationAttack = value; }

};
