// Fill out your copyright notice in the Description page of Project Settings.
#include "UnrealPortfolio.h"
#include "BossAIController.h"

#include "BossCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"

#include "BTTask_BossChase.h"

UBTTask_BossChase::UBTTask_BossChase()
{
	NodeName = TEXT("BossChase");

	//TickTask 함수가 실행됨
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_BossChase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (Target == nullptr) return EBTNodeResult::Failed;

	//컨트롤러를 Target으로 이동
	UAIBlueprintHelperLibrary::SimpleMoveToActor(BossAIController, Target);
	return EBTNodeResult::InProgress; //진행 중 반환
}

void UBTTask_BossChase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (Target == nullptr) return;
	float dist = FVector::Dist2D(Target->GetActorLocation(), BossAIController->GetPawn()->GetActorLocation());
	float AttackRange = Boss->GetAttackRange();

	if (dist <= AttackRange)
	{
		//행동 종료 후 성공 반환
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
