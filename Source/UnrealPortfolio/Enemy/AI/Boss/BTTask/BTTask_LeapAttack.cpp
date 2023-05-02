// Fill out your copyright notice in the Description page of Project Settings.
#include "UnrealPortfolio.h"
#include "AIController.h"


#include "BossCharacter.h"

#include "Kismet/KismetMathLibrary.h"

#include "BTTask_LeapAttack.h"

UBTTask_LeapAttack::UBTTask_LeapAttack()
{
	NodeName = TEXT("LeapAttack");

	//TickTask 함수가 실행됨
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_LeapAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	fAccumulateTickTime = 0.0f;

	bIsLeapAttackTaskExeFlag = false;

	Boss->SetLandingLocation(Target);
	
	return EBTNodeResult::InProgress;
}

void UBTTask_LeapAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	fAccumulateTickTime += DeltaSeconds;

	if (!bIsLeapAttackTaskExeFlag && fAccumulateTickTime >= Boss->GetLeapAttackCastingTime())
	{
		bIsLeapAttackTaskExeFlag = true;
		Boss->LeapAttackStart();
	}

	if (bIsLeapAttackTaskExeFlag && !Boss->GetIsBeingLeapAttack())
	{
		//Montage_GetIsStopped : 몽타주가 끝낫는지 여부 반환
		//GetCurrentMontage : 현재 재생중인 몽타주

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}


}
