// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "BossCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "BossAnimInstance.h"
UBossAnimInstance::UBossAnimInstance()
{
	speed = 0.0f;
	direction = 0.0f;
}
void UBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (TryGetPawnOwner() == nullptr) return;
	Boss = Cast<ABossCharacter>(TryGetPawnOwner());
}

void UBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!IsValid(Boss)) return;

	//Enemy의 속도
	speed = Boss->GetVelocity().Size2D();

	//Enemy 방향
	direction = CalculateDirection(Boss->GetVelocity(), Boss->GetControlRotation());

	//사망여부
	isDeath = Boss->GetIsDeath();

	//페이즈
	Phase = Boss->GetBossPhase();
}

void UBossAnimInstance::AnimNotify_NextAttack()
{
	if (!IsValid(Boss)) return;
	Boss->NextAttack();
}

void UBossAnimInstance::AnimNotify_EndAttack()
{
	if (!IsValid(Boss)) return;
	Boss->InitBossAttackNum();
	Boss->SetIsBeingAttack(false);
	Boss->SetIsHadouKenAttack(false);
}

void UBossAnimInstance::AnimNotify_LeapAttackStart()
{
	Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->DoCameraShake();
	
}

void UBossAnimInstance::AnimNotify_LoopEndGroundSmash()
{
	if (!IsValid(Boss)) return;
	if (Boss->GetCharacterMovement()->CurrentFloor.IsWalkableFloor())
	{
		Boss->LeapAttackLoopEnd();
	}
}

void UBossAnimInstance::AnimNotify_EndGroundSmash()
{
	if (!IsValid(Boss)) return;
	Boss->SetIsBeingLeapAttack(false);
}

void UBossAnimInstance::AnimNotify_Damage()
{
	if (!IsValid(Boss)) return;
	Boss->SendDamage();
	if (Boss->GetIsHadouKenAttack())
	{
		Boss->ShotSpell();
	}
}

void UBossAnimInstance::AnimNotify_GroundSmashDamage()
{
	if (!IsValid(Boss)) return;
	Boss->RadialDamage();
}

//
