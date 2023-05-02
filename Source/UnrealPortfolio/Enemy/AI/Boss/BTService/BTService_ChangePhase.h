// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_ChangePhase.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UBTService_ChangePhase : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_ChangePhase();

protected:
	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;

private:
	
};
