// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"

#include "EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyAnimInstance.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{
	speed = 0.0f;
	direction = 0.0f;
}
void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (TryGetPawnOwner() == nullptr) return;

	MyEnemy = Cast<AEnemyCharacter>(TryGetPawnOwner());
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!IsValid(MyEnemy)) return;

	//Enemy의 속도
	speed = MyEnemy->GetVelocity().Size2D();
	//Enemy 방향
	direction = CalculateDirection(MyEnemy->GetVelocity(), MyEnemy->GetControlRotation());

	//공중 여부
	isAir = (MyEnemy->GetMovementComponent()->IsFalling() || MyEnemy->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying);

	isFalling = MyEnemy->GetIsFalling();

	isHitFlying = MyEnemy->GetIsHitFlying(); //공중 피격 판정

	downTimer = MyEnemy->GetDownTimer(); //공중 피격 후 다운 지속 시간

	isBeingGrabbed = MyEnemy->GetIsBeingGrabbed(); //잡힌중인지

	isDown = MyEnemy->GetIsDown(); //다운 상태인지

	isBeingKnockBack = MyEnemy->GetIsBeingKnockBack();

	isFallingToFloor = MyEnemy->GetFallingToFloor();

	isDeath = MyEnemy->GetIsDeath();
}

void UEnemyAnimInstance::AnimNotify_EnemyAttack()
{
	if (!IsValid(MyEnemy)) return;
	MyEnemy->SendDamage();
}

void UEnemyAnimInstance::AnimNotify_EndLevelStart()
{

}
