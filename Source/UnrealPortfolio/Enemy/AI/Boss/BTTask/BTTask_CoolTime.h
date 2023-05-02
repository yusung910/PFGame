// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_BossAction.h"
#include "BTTask_CoolTime.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UBTTask_CoolTime : public UBTTask_BossAction
{
	GENERATED_BODY()

public:
	UBTTask_CoolTime();


protected:
	virtual EBTNodeResult::Type ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	float fCoolTimeLimit; //쿨타임 제한시간

	float fAccumulateTickTime; //틱타임 축적 변수

	float fStartCoolTimeLimit;
	float fEndCoolTimeLimit;
};
