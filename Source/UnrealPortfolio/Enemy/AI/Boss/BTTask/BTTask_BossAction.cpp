// Fill out your copyright notice in the Description page of Project Settings.
#include "UnrealPortfolio.h"
#include "AIController.h"
#include "BlackboardKeys.h"
#include "BossCharacter.h"
#include "BossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CommonCharacter.h"
#include "BTTask_BossAction.h"

UBTTask_BossAction::UBTTask_BossAction()
{
	//TickTask 함수가 실행됨
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_BossAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;
	if (OwnerComp.GetAIOwner()->GetPawn() == nullptr) return EBTNodeResult::Failed;

	Boss = Cast<ABossCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	BossAIController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
	Target = Cast<AActor>(BossAIController->GetBlackboardComponent()->GetValueAsObject(BBKeys::target));

	return EBTNodeResult::InProgress;
}

void UBTTask_BossAction::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	IS_NULL(OwnerComp.GetAIOwner());
	IS_NULL(OwnerComp.GetAIOwner()->GetPawn());
}
