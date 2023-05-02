// Fill out your copyright notice in the Description page of Project Settings.
#include "BlackboardKeys.h"

#include "UnrealPortfolio.h"

#include "EnemyAIController.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"

#include "BTTask_Chase.h"

UBTTask_Chase::UBTTask_Chase()
{
	NodeName = TEXT("Chase");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AEnemyAIController* EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (EnemyController == nullptr) return EBTNodeResult::Failed;

	//�����忡 ����� Ÿ���� �����´�
	AActor* Target = Cast<AActor>(EnemyController->GetBlackboardComponent()->GetValueAsObject(BBKeys::target));

	if (Target == nullptr) return EBTNodeResult::Failed;

	//��Ʈ�ѷ��� Target���� �̵�
	UAIBlueprintHelperLibrary::SimpleMoveToActor(EnemyController, Target);

	return EBTNodeResult::InProgress; //���� �� ��ȯ
}

void UBTTask_Chase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyAIController* EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (EnemyController == nullptr) return;

	AActor* Target = Cast<AActor>(EnemyController->GetBlackboardComponent()->GetValueAsObject(BBKeys::target));

	if (Target == nullptr) return;

	if (FVector::Dist2D(Target->GetActorLocation(), EnemyController->GetPawn()->GetActorLocation()) < 200.f)
	{
		//�ൿ ���� �� ���� ��ȯ
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}