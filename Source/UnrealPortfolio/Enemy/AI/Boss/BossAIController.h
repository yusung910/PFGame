// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Perception/AIPerceptionTypes.h"

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossAIController.generated.h"



/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API ABossAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ABossAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaTime) override;
private:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
		class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
		class UBehaviorTree* BTree;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
		class ABossCharacter* Boss;

	bool isFindPlayer = false;

private:
	UFUNCTION()
		void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);
};
