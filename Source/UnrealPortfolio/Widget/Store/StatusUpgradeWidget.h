// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FUpgradeCostTableData.h"

#include "StatusUpgradeWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UStatusUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UStatusUpgradeWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeOnInitialized() override;

	UFUNCTION()
		void HpLevelUp();

	UFUNCTION()
		void ManaLevelUp();


	void SetStatusUpgradeCostInfo();

private:
	UPROPERTY()
		class UMainGameInstance* gi;

	UPROPERTY()
		class AMainGameMode* MainGM;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		class UDataTable* StatusCostList;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* CurrentHP;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* NextHP;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* CostHP;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnHpUp;

	
	FStatusUpgradeCostTableData* HpUpgradeCostInfo;


	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* CurrentMana;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* NextMana;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* CostMana;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnManaUp;

	FStatusUpgradeCostTableData* ManaUpgradeCostInfo;
};
