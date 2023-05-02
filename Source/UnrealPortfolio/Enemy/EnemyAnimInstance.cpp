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

	//Enemy�� �ӵ�
	speed = MyEnemy->GetVelocity().Size2D();
	//Enemy ����
	direction = CalculateDirection(MyEnemy->GetVelocity(), MyEnemy->GetControlRotation());

	//���� ����
	isAir = (MyEnemy->GetMovementComponent()->IsFalling() || MyEnemy->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying);

	isFalling = MyEnemy->GetIsFalling();

	isHitFlying = MyEnemy->GetIsHitFlying(); //���� �ǰ� ����

	downTimer = MyEnemy->GetDownTimer(); //���� �ǰ� �� �ٿ� ���� �ð�

	isBeingGrabbed = MyEnemy->GetIsBeingGrabbed(); //����������

	isDown = MyEnemy->GetIsDown(); //�ٿ� ��������

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
