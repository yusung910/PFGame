// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "MainPlayerController.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "UnrealPortfolio.h"
#include "PlayerEnumActor.h"
#include "EnemyCharacter.h"
#include "WeaponActor.h"
#include "PlayerCharacter.h"

#include "PlayerAnimInstance.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	Speed = 0.f;
	Direction = 0.f;
	isAir = false;

}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	IS_NULL(TryGetPawnOwner());
	MyPlayer = Cast<APlayerCharacter>(TryGetPawnOwner());

}


void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!IsValid(MyPlayer)) return;

	//����(Z)�� �� ������ ���� Size2D
	Speed = MyPlayer->GetVelocity().Size2D();

	//�÷��̾��� �����, ī�޶��� ���� ���� ���� ��ȯ
	//������ Yaw���� �����Ͽ� ����
	float directionYaw = MyPlayer->GetActorRotation().Yaw;
	if (MyPlayer->GetIsLockOnEnemy())
	{
		//���溤�Ͱ� �ƴ� �Ϲ� �÷��̾� ��ġ ���͸� ����ؾ���.
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(MyPlayer->GetActorLocation(), MyPlayer->GetLockEnemy()->GetActorLocation());
		directionYaw = rot.Yaw;
	}
	
	Direction = CalculateDirection(MyPlayer->GetVelocity(), FRotator(0, directionYaw, 0));


	//�߶������� ����
	isAir = MyPlayer->GetMovementComponent()->IsFalling();

	//ĳ������ ���� ���¸� ����
	MyPlayer->SetIsAir(isAir);

	//�߰� ���� �������� ����
	isAdditionJump = MyPlayer->GetIsAdditionJump();

	//�÷��̾� ĳ������ ���� ���� ���� ����
	currentWeapon = MyPlayer->GetCurrentWeaponType();

	//�÷��̾ ����������?
	isAttack = MyPlayer->GetIsAttack();

	//�÷��̾ ����������
	isFalling = MyPlayer->GetIsFalling();

	//���ʹ� ��ü�� ���� ������
	isEnemyLockOn = MyPlayer->GetIsLockOnEnemy();

	//���� ��� �����鼭 enemy�� ���� �ϰ� ���� ��� enemy�� pitch���� �����´�.
	lockOnPitch = MyPlayer->GetLookAtPitch();

	//��� ��������
	isBlock = MyPlayer->GetIsBlock();

	//���� ĳ������ �����Ʈ ���
	isFlyingMode = (MyPlayer->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying);
}



void UPlayerAnimInstance::AnimNotify_Damage()
{
	if (!IsValid(MyPlayer)) return;
	MyPlayer->GetCurrentWeaponObj()->SendDamage();
	MyPlayer->SetIsbeingGrapped(false);

}

void UPlayerAnimInstance::AnimNotify_KnockBackDamage()
{
	//�÷��̾� ���� ��ü���� �浹�̺�Ʈ�� ���� �������� ���� ���ʹ� ��ü�� �ڷ� ����������.
	if (!IsValid(MyPlayer)) return;
	MyPlayer->GetCurrentWeaponObj()->KnockBackDamage();
	MyPlayer->SetIsbeingGrapped(false);
}

void UPlayerAnimInstance::AnimNotify_EndHit()
{
	if (!IsValid(MyPlayer)) return;

	MyPlayer->SetIsAttack(false);
	MyPlayer->SetIsMove(true);
	MyPlayer->SetIsDashAttack(false);
	MyPlayer->SetIsDodge(false);
	MyPlayer->SetIsATFAttack(false);
	MyPlayer->SetIsbeingGrapped(false);
	MyPlayer->SetIsUpperAttack(false);
	MyPlayer->GetCurrentWeaponObj()->ResetAttackCount();
}

void UPlayerAnimInstance::AnimNotify_GetUp()
{
	if (!IsValid(MyPlayer)) return;
	MyPlayer->SetIsAttack(false);
	MyPlayer->SetIsMove(true);
	MyPlayer->SetIsDashAttack(false);
	MyPlayer->SetIsDodge(false);
	MyPlayer->SetIsATFAttack(false);
	MyPlayer->SetIsbeingGrapped(false);
	MyPlayer->SetIsUpperAttack(false);
	MyPlayer->SetIsDown(false);
	MyPlayer->SetIsBeingGetUp(false);
}

void UPlayerAnimInstance::AnimNotify_StartDownLoop()
{
	if (!IsValid(MyPlayer)) return;
}

void UPlayerAnimInstance::AnimNotify_DownEnd()
{
	if (!IsValid(MyPlayer)) return;
}

void UPlayerAnimInstance::AnimNotify_BlockStart()
{
	if (!IsValid(MyPlayer) &&
		!MyPlayer->GetIsAirPosition()) return;
	//��� ������ ��� �̼��� �����.
	MyPlayer->GetCharacterMovement()->MaxWalkSpeed = MyPlayer->GetMoveSpeed() * 0.2f;
	MyPlayer->StopAnimMontage();
	MyPlayer->GetCharacterMovement()->StopMovementImmediately();
	MyPlayer->SetCurrentWeaponType(EPlayerWeapon::SWORD);
	MyPlayer->currentWeapon->ResetAttackCount();
	MyPlayer->DoBlock("BlockLoop");
	MyPlayer->SetIsBlock(true);
}

void UPlayerAnimInstance::AnimNotify_BlockEnd()
{
	if (!IsValid(MyPlayer) && MyPlayer->GetIsBlock()) return;
	MyPlayer->SetIsBlock(false);
	MyPlayer->SetIsMove(true);
	MyPlayer->SetIsAttack(false);
	MyPlayer->GetCharacterMovement()->MaxWalkSpeed = MyPlayer->GetMoveSpeed();
}

void UPlayerAnimInstance::AnimNotify_BlockHitEnd()
{
	if (!IsValid(MyPlayer) &&
		!MyPlayer->GetIsDown() &&
		!MyPlayer->GetIsAirPosition()) return;
	MyPlayer->DoBlock("BlockLoop");
}


void UPlayerAnimInstance::AnimNotify_NextAttack()
{
	if (!IsValid(MyPlayer)) return;
	MyPlayer->GetCurrentWeaponObj()->NextAttack();
}

void UPlayerAnimInstance::AnimNotify_EndAttack()
{
	if (!IsValid(MyPlayer)) return;
	MyPlayer->SetIsAttack(false);
	MyPlayer->GetCurrentWeaponObj()->ResetAttackCount();
	MyPlayer->SetIsMove(true);
	MyPlayer->SetIsbeingGrapped(false);
	MyPlayer->SetIsKnockBackAttack(false);
}

void UPlayerAnimInstance::AnimNotify_SetAir()
{
	if (!IsValid(MyPlayer)) return;
	MyPlayer->LaunchCharacter(FVector(0, 0, 2000.f), false, false);
	MyPlayer->SetIsAir(true);
}

void UPlayerAnimInstance::AnimNotify_EndUpperAttack()
{
	if (!IsValid(MyPlayer)) return;
	MyPlayer->SetIsAttack(false);
	MyPlayer->SetIsUpperAttack(false);
	MyPlayer->SetIsFloorToAirAttack(false);
	MyPlayer->SetIsMove(true);
	MyPlayer->SetIsbeingGrapped(false);
	MyPlayer->GetCurrentWeaponObj()->SetIsLevitationAttack(false);
}


void UPlayerAnimInstance::AnimNotify_EndAirAttack()
{
	if (!IsValid(MyPlayer)) return;
	MyPlayer->SetIsAttack(false);
	MyPlayer->SetIsMove(true);
	MyPlayer->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Falling;
	MyPlayer->SetIsbeingGrapped(false);
	MyPlayer->GetCurrentWeaponObj()->ResetAttackCount();
}

void UPlayerAnimInstance::AnimNotify_FinalAirAttack()
{
	if (!IsValid(MyPlayer)) return;
	MyPlayer->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Falling;

	FTimerHandle WaitHandle;
	float WaitTime = 0.5f; //�ð��� �����ϰ�
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			MyPlayer->SetIsAttack(false);
			MyPlayer->SetIsMove(true);
			MyPlayer->GetCurrentWeaponObj()->ResetAttackCount();
			MyPlayer->SetIsKnockBackAttack(false);
	}), WaitTime, false); //�ݺ��� ���⼭ �߰� ������ ������ ��������

}

void UPlayerAnimInstance::AnimNotify_EndDashAttack()
{
	if (!IsValid(MyPlayer)) return;

	MyPlayer->SetIsAttack(false);
	MyPlayer->SetIsMove(true);
	MyPlayer->SetIsDashAttack(false);
	MyPlayer->SetIsKnockBackAttack(false);
}

void UPlayerAnimInstance::AnimNotify_DodgeEnd()
{
	if (!IsValid(MyPlayer)) return;

	MyPlayer->SetIsMove(true);
	MyPlayer->SetIsDodge(false);
	MyPlayer->SetIsAttack(false);
}

void UPlayerAnimInstance::AnimNotify_NextATFAttack()
{
	if (!IsValid(MyPlayer)) return;
	MyPlayer->GetCurrentWeaponObj()->NextATFAttack();
}

void UPlayerAnimInstance::AnimNotify_EndATFLoop()
{
	if (!IsValid(MyPlayer)) return;

	if (!MyPlayer->GetIsAir())
	{
		MyPlayer->GetCurrentWeaponObj()->EndATFLoop();
	}
}

void UPlayerAnimInstance::AnimNotify_EndATF()
{
	if (!IsValid(MyPlayer)) return;

	MyPlayer->SetIsAttack(false);
	MyPlayer->SetIsMove(true);
	MyPlayer->SetIsATFAttack(false);
	MyPlayer->SetIsbeingGrapped(false);
}

void UPlayerAnimInstance::AnimNotify_TargetBound()
{
	if (!IsValid(MyPlayer)) return;
	if (!MyPlayer->GetIsLockOnEnemy()) return;
	ACommonCharacter* enemy = Cast<ACommonCharacter>(MyPlayer->GetLockEnemy());
	enemy->FallingToFloor();
	enemy->SetIsHitFlying(false);
}

void UPlayerAnimInstance::AnimNotify_EndShoot()
{
	if (!IsValid(MyPlayer)) return;
	MyPlayer->SetIsAttack(false);
	MyPlayer->SetIsDashAttack(false);
	MyPlayer->SetIsbeingGrapped(false);
	MyPlayer->SetIsFloorToAirAttack(false);

}

void UPlayerAnimInstance::AnimNotify_GrapEnd()
{
	if (!IsValid(MyPlayer)) return;
	MyPlayer->SetIsAttack(false);
	MyPlayer->SetIsMove(true);
	MyPlayer->SetIsbeingGrapped(false);
	MyPlayer->SetIsUpperAttack(false);
	MyPlayer->SetIsDashAttack(false);

	if(MyPlayer->GetIsAirPosition())
	MyPlayer->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Falling;

	if (!MyPlayer->GetCurrentWeaponObj()) return;
	MyPlayer->GetCurrentWeaponObj()->SetIsLevitationAttack(false);
	MyPlayer->SetIsKnockBackAttack(false);
}

void UPlayerAnimInstance::AnimNotify_DoGrap()
{
	if (!IsValid(MyPlayer)) return;
	MyPlayer->DoGrap();
}





