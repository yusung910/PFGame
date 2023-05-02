// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Chase.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UBTTask_Chase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_Chase();

protected:
	virtual EBTNodeResult::Type ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
