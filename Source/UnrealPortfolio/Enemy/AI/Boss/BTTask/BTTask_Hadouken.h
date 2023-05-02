// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_BossAction.h"
#include "BTTask_Hadouken.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UBTTask_Hadouken : public UBTTask_BossAction
{
	GENERATED_BODY()
	
public:
	UBTTask_Hadouken();

protected:
	virtual EBTNodeResult::Type ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
