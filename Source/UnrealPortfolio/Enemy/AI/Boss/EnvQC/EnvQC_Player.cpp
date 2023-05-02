// Fill out your copyright notice in the Description page of Project Settings.

#include "EnvQC_Player.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

#include "Kismet/GameplayStatics.h"

void UEnvQC_Player::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	UEnvQueryItemType_Actor::SetContextHelper(ContextData, Cast<AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)));
}
