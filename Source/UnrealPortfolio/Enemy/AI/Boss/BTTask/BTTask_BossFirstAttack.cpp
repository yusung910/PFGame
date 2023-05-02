// Fill out your copyright notice in the Description page of Project Settings.
#include "UnrealPortfolio.h"
#include "AIController.h"

#include "BossCharacter.h"

#include "BTTask_BossFirstAttack.h"

UBTTask_BossFirstAttack::UBTTask_BossFirstAttack()
{
	NodeName = TEXT("FirstAttack");

	//TickTask 함수가 실행됨
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_BossFirstAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	//보스 캐릭터의 어택 함수 실행
	Boss->Attack();

	return EBTNodeResult::InProgress;
}

void UBTTask_BossFirstAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!Boss->GetIsBeingAttack())
	{
		//Montage_GetIsStopped : 몽타주가 끝낫는지 여부 반환
		//GetCurrentMontage : 현재 재생중인 몽타주
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
