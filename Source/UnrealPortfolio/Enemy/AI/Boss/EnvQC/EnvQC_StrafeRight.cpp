// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"

#include "EnvQC_StrafeRight.h"

#include "BossAIController.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "PlayerCharacter.h"
#include "BossCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UEnvQC_StrafeRight::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	UObject* QuerierObject = QueryInstance.Owner.Get();

	IS_NULL(QuerierObject);

	ABossAIController* BossAIController = Cast<ABossAIController>((Cast<AActor>((QueryInstance.Owner).Get())->GetInstigatorController()));

	ABossCharacter* Boss = Cast<ABossCharacter>(QuerierObject);

	APlayerCharacter* Player = Cast<APlayerCharacter>(BossAIController->GetBlackboardComponent()->GetValueAsObject("target"));

	FRotator PlayerToBossLookRot = UKismetMathLibrary::FindLookAtRotation(Boss->GetActorLocation(), Player->GetActorLocation());

	FRotator LeftRot = UKismetMathLibrary::MakeRotator(0, 0, PlayerToBossLookRot.Yaw + 90);
	FVector LeftForwardVec = UKismetMathLibrary::GetForwardVector(LeftRot * Boss->GetAttackRange());

	UEnvQueryItemType_Point::SetContextHelper(ContextData, LeftForwardVec + Boss->GetActorLocation());
}
