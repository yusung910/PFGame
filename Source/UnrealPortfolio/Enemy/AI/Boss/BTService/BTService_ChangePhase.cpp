// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"

#include "AIController.h"
#include "BossCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


#include "BTService_ChangePhase.h"


UBTService_ChangePhase::UBTService_ChangePhase()
{
	NodeName = TEXT("ChangePhase");
}

void UBTService_ChangePhase::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);
	IS_NULL(SearchData.OwnerComp.GetAIOwner());
	ABossCharacter* Boss = Cast<ABossCharacter>(SearchData.OwnerComp.GetAIOwner()->GetPawn());

	IS_NULL(Boss);
	//페이즈에 따른 보스 스텟 변경
}
