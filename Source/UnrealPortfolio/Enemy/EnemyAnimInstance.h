// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UEnemyAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "EnemyCharacter", meta = (AllowPrivateAccess = true))
		float speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "EnemyCharacter", meta = (AllowPrivateAccess = true))
		float direction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "EnemyCharacter", meta = (AllowPrivateAccess = true))
		float HitYaw;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "EnemyCharacter", meta = (AllowPrivateAccess = true))
		bool isAir;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "EnemyCharacter", meta = (AllowPrivateAccess = true))
		class AEnemyCharacter* MyEnemy;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "EnemyCharacter", meta = (AllowPrivateAccess = true))
		bool isHitFlying;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "EnemyCharacter", meta = (AllowPrivateAccess = true))
		bool isFalling;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "EnemyCharacter", meta = (AllowPrivateAccess = true))
		float downTimer = 0.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "EnemyCharacter", meta = (AllowPrivateAccess = true))
		bool isBeingGrabbed = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "EnemyCharacter", meta = (AllowPrivateAccess = true))
		bool isDown = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "EnemyCharacter", meta = (AllowPrivateAccess = true))
		bool isBeingKnockBack = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "EnemyCharacter", meta = (AllowPrivateAccess = true))
		bool isFallingToFloor = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "EnemyCharacter", meta = (AllowPrivateAccess = true))
		bool isDeath = false;

protected:
	UFUNCTION()
		void AnimNotify_EnemyAttack();

	UFUNCTION()
		void AnimNotify_EndLevelStart();

};
