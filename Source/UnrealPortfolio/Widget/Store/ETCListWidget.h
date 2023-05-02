// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ETCListWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UETCListWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	void HealItemCostSet();

	UFUNCTION()
		void BuyHealHp();

	UFUNCTION()
		void BuyHealMana();

	UFUNCTION()
		void BuyHealHpMana();

private:
	UPROPERTY()
		class UMainGameInstance* gi;

	UPROPERTY()
		class AMainGameMode* MainGM;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnHealHp;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* HpHealCostTxt;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnHealMana;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* ManaHealCostTxt;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnHPManaHeal;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* HpManaHealCostTxt;


	float HpHealCost = 50;
	float ManaHealCost = 55;
	float HpManaHealCost = 90;

	
};
