// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Perception/AIPerceptionTypes.h"

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public: 
	AEnemyAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaTime) override;
private:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
		class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
		class UBehaviorTree* BTree;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
		class AEnemyCharacter* Enemy;

private:
	UFUNCTION()
		void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);
	
	//AI가 넘어진 후 다시 일어났을 때 일정 시간 이후에 블랙보드 값을 변경하기 위한 타이머 변수
	float DownTimer;
};
