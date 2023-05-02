// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"
#include "AIController.h"

#include "BossCharacter.h"
#include "BTTask_Cast.h"

UBTTask_Cast::UBTTask_Cast()
{
	NodeName = TEXT("Cast");

	//TickTask �Լ��� �����
	bNotifyTick = true;
}
EBTNodeResult::Type UBTTask_Cast::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	return EBTNodeResult::InProgress;
}

void UBTTask_Cast::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

}
