// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EBossStatus.h"

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BossAction.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UBTTask_BossAction : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_BossAction();

protected:
	virtual EBTNodeResult::Type ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY()
		class ABossCharacter* Boss;
	
	UPROPERTY()
		class AActor* Target;

	UPROPERTY()
		class ABossAIController* BossAIController;
};
