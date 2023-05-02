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

	//TickTask �Լ��� �����
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_BossChase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (Target == nullptr) return EBTNodeResult::Failed;

	//��Ʈ�ѷ��� Target���� �̵�
	UAIBlueprintHelperLibrary::SimpleMoveToActor(BossAIController, Target);
	return EBTNodeResult::InProgress; //���� �� ��ȯ
}

void UBTTask_BossChase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (Target == nullptr) return;
	float dist = FVector::Dist2D(Target->GetActorLocation(), BossAIController->GetPawn()->GetActorLocation());
	float AttackRange = Boss->GetAttackRange();

	if (dist <= AttackRange)
	{
		//�ൿ ���� �� ���� ��ȯ
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
