// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackboardKeys.h"

#include "UnrealPortfolio.h"
#include "BossAIController.h"

#include "BossCharacter.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"

#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"

#include "Kismet/KismetMathLibrary.h"

#include "NavigationSystem.h"

#include "BTService_BossStrafeTask.h"

UBTService_BossStrafeTask::UBTService_BossStrafeTask()
{
	NodeName = TEXT("StrafeTask");
	bNotifyBecomeRelevant = true;
}
void UBTService_BossStrafeTask::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);
}

void UBTService_BossStrafeTask::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	IS_NULL(OwnerComp.GetAIOwner());
	BossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	Boss = Cast<ABossCharacter>(BossController->GetPawn());

	bool isFindTarget = BossController->GetBlackboardComponent()->GetValueAsBool("isFindPlayer");

	if (isFindTarget)
	{
		DoStrafeQuery();
	}
	else
	{
		FNavLocation NavPos; //네비게이션 기반 위치저장 구조체

		//현재 월드에 있는 네비게이션 시스템을 가져온다
		UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

		bool bLocalRndPosResult = NavSys->GetRandomPointInNavigableRadius(Boss->GetActorLocation(), SearchRadius, NavPos);

		BossController->GetBlackboardComponent()->SetValueAsVector(BBKeys::AvoidLoc, NavPos.Location);
	}
}


void UBTService_BossStrafeTask::DoStrafeQuery()
{
	AActor* Player = Cast<AActor>(BossController->GetBlackboardComponent()->GetValueAsObject("target"));

	FVector PlayerToBossDirVec = UKismetMathLibrary::GetDirectionUnitVector(Player->GetActorLocation(), Boss->GetActorLocation());

	float BossToPlayerDot = UKismetMathLibrary::Dot_VectorVector(Player->GetActorRightVector().GetSafeNormal(), PlayerToBossDirVec);

	FEnvQueryRequest HidingSpotQueryRequest;
	if (BossToPlayerDot <= 0.0f)
	{
		//right
		HidingSpotQueryRequest = FEnvQueryRequest(RightEQS, Boss);
	}
	else
	{
		//left
		HidingSpotQueryRequest = FEnvQueryRequest(LeftEQS, Boss);
	}


	HidingSpotQueryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &UBTService_BossStrafeTask::MoveToQueryResult);
}

void UBTService_BossStrafeTask::MoveToQueryResult(TSharedPtr<FEnvQueryResult> result)
{
	if (result->IsSuccsessful()) {
		FVector MoveVec = result->GetItemAsLocation(0);
		BossController->GetBlackboardComponent()->SetValueAsVector(BBKeys::AvoidLoc, MoveVec);
	}
}