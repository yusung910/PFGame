// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackboardKeys.h"
#include "EnemyAIController.h"
#include "CommonCharacter.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BTTask_TurnToTarget.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto CmnCharacter = Cast<ACommonCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (CmnCharacter == nullptr) return EBTNodeResult::Failed;

	auto Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKeys::target));
	if (Target == nullptr) return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - CmnCharacter->GetActorLocation();

	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

	//CmnCharacter->SetActorRotation(FMath::RInterpTo(CmnCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	CmnCharacter->SetActorRotation(TargetRot);

	return EBTNodeResult::Succeeded;
}
