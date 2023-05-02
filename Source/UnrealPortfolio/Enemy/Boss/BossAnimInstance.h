// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UBossAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
		void AnimNotify_Damage();

	UFUNCTION()
		void AnimNotify_GroundSmashDamage();

	UFUNCTION()
		void AnimNotify_NextAttack();

	UFUNCTION()
		void AnimNotify_EndAttack();

	UFUNCTION()
		void AnimNotify_LeapAttackStart();

	UFUNCTION()
		void AnimNotify_LoopEndGroundSmash();

	UFUNCTION()
		void AnimNotify_EndGroundSmash();

private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "BossCharacter", meta = (AllowPrivateAccess = true))
		float speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "BossCharacter", meta = (AllowPrivateAccess = true))
		float direction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "BossCharacter", meta = (AllowPrivateAccess = true))
		class ABossCharacter* Boss;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "BossCharacter", meta = (AllowPrivateAccess = true))
		int32 Phase;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "BossCharacter", meta = (AllowPrivateAccess = true))
		bool isDeath;
};
