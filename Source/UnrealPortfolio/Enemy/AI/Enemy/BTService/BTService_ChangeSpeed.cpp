// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "BTService_ChangeSpeed.h"

UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{
	NodeName = TEXT("ChangeSpeed");
}

void UBTService_ChangeSpeed::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);
	if (SearchData.OwnerComp.GetAIOwner() == nullptr) return;
	ACharacter* Enemy = Cast<ACharacter>(SearchData.OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy == nullptr) return;
	Enemy->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}
