// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OrbStructActor.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OrbsEntry.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UOrbsEntry : public UUserWidget
{
	GENERATED_BODY()
public:

	UFUNCTION()
		void InitOrbsData(UOrbsManagerComponent* comp, FOrbsDynamicData data, int32 idx);

	UFUNCTION()
		void RefreshOrbData();

private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* OrbValue; //게임 재화 수치

	UPROPERTY(meta = (BindWidget))
		class UImage* OrbImageBg;

	UPROPERTY(meta = (BindWidget))
		class UImage* OrbImageColor;

	UPROPERTY()
		class UOrbsManagerComponent* OrbManagerComponent;

	UPROPERTY()
		FOrbsDynamicData OrbDynamicData;

	UPROPERTY()
		int32 OrbsDataIdx;

};
