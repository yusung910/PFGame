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

	//높이(Z)를 뺀 벡터의 길이 Size2D
	Speed = MyPlayer->GetVelocity().Size2D();

	//플레이어의 방향과, 카메라의 방향 차이 각을 반환
	//방향을 Yaw값만 변경하여 적용
	float directionYaw = MyPlayer->GetActorRotation().Yaw;
	if (MyPlayer->GetIsLockOnEnemy())
	{
		//전방벡터가 아닌 일반 플레이어 위치 벡터를 사용해야함.
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(MyPlayer->GetActorLocation(), MyPlayer->GetLockEnemy()->GetActorLocation());
		directionYaw = rot.Yaw;
	}
	
	Direction = CalculateDirection(MyPlayer->GetVelocity(), FRotator(0, directionYaw, 0));


	//추락중인지 여부
	isAir = MyPlayer->GetMovementComponent()->IsFalling();

	//캐릭터의 점프 상태를 저장
	MyPlayer->SetIsAir(isAir);

	//추가 점프 가능한지 여부
	isAdditionJump = MyPlayer->GetIsAdditionJump();

	//플레이어 캐릭터의 현재 무기 정보 저장
	currentWeapon = MyPlayer->GetCurrentWeaponType();

	//플레이어가 공격중인지?
	isAttack = MyPlayer->GetIsAttack();

	//플레이어가 낙하중인지
	isFalling = MyPlayer->GetIsFalling();

	//에너미 객체를 락온 중인지
	isEnemyLockOn = MyPlayer->GetIsLockOnEnemy();

	//총을 들고 있으면서 enemy를 락온 하고 있을 경우 enemy의 pitch값을 가져온다.
	lockOnPitch = MyPlayer->GetLookAtPitch();

	//블락 상태인지
	isBlock = MyPlayer->GetIsBlock();

	//현재 캐릭터의 무브먼트 모드
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
	//플레이어 무기 객체에서 충돌이벤트를 통해 데미지를 입은 에너미 객체를 뒤로 날려버린다.
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
	//블락 상태일 경우 이속을 늦춘다.
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
	float WaitTime = 0.5f; //시간을 설정하고
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			MyPlayer->SetIsAttack(false);
			MyPlayer->SetIsMove(true);
			MyPlayer->GetCurrentWeaponObj()->ResetAttackCount();
			MyPlayer->SetIsKnockBackAttack(false);
	}), WaitTime, false); //반복도 여기서 추가 변수를 선언해 설정가능

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





