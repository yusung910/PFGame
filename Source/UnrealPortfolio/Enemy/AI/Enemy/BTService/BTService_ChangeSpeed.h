// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_ChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UBTService_ChangeSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_ChangeSpeed();

protected:
	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
		float MoveSpeed = 500.f;
};
