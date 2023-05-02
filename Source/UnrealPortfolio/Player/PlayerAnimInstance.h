// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PlayerEnumActor.h"
#include "FPlayerControllerInputFlag.h"

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPlayerAnimInstance();

	//�������Ʈ �ִϸ��̼� �ʱ�ȭ �Լ�
	virtual void NativeInitializeAnimation() override;

	//�������Ʈ �ִϸ��̼� ƽ �Լ�
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//�������Ʈ ������Ʈ �ִϸ��̼� �Լ�

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool isAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool isFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool isAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool isAdditionJump;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool isEnemyLockOn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool isBlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float lockOnPitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		EPlayerWeapon currentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		class APlayerCharacter* MyPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool isFlyingMode;


protected:
	//�𸮾����� Damage��� �̸��� ���̷��� ��Ƽ���̰� ���� �Ǹ� 
	//AnimNotify_Damage�Լ��� ã�� �����ϰ� �ȴ�
	//���ξ� 'AnimNotify_'�� ������Ѵ�
	UFUNCTION()
		void AnimNotify_Damage();

	UFUNCTION()
		void AnimNotify_NextAttack();

	UFUNCTION()
		void AnimNotify_EndAttack();

	UFUNCTION()
		void AnimNotify_SetAir();

	UFUNCTION()
		void AnimNotify_EndUpperAttack();

	UFUNCTION()
		void AnimNotify_EndAirAttack();

	UFUNCTION()
		void AnimNotify_FinalAirAttack();

	UFUNCTION()
		void AnimNotify_EndDashAttack();

	UFUNCTION()
		void AnimNotify_DodgeEnd();

	UFUNCTION()
		void AnimNotify_NextATFAttack();

	UFUNCTION()
		void AnimNotify_EndATFLoop();

	UFUNCTION()
		void AnimNotify_EndATF();

	UFUNCTION()
		void AnimNotify_TargetBound();

	UFUNCTION()
		void AnimNotify_EndShoot();

	UFUNCTION()
		void AnimNotify_GrapEnd();

	UFUNCTION()
		void AnimNotify_DoGrap();

	UFUNCTION()
		void AnimNotify_KnockBackDamage();

	UFUNCTION()
		void AnimNotify_EndHit();

	UFUNCTION()
		void AnimNotify_GetUp();

	UFUNCTION()
		void AnimNotify_StartDownLoop();

	UFUNCTION()
		void AnimNotify_DownEnd();

	UFUNCTION()
		void AnimNotify_BlockStart();

	UFUNCTION()
		void AnimNotify_BlockEnd();

	UFUNCTION()
		void AnimNotify_BlockHitEnd();

};