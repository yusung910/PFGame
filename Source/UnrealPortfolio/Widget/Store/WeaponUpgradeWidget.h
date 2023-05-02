// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "FUpgradeCostTableData.h"
#include "WeaponUpgradeWidget.generated.h"


/**
 *
 */
UCLASS()
class UNREALPORTFOLIO_API UWeaponUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UWeaponUpgradeWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;

	void SetWeaponUpgradeCostInfo();

	UFUNCTION()
		void SwordLevelUp();
	UFUNCTION()
		void GunLevelUp();
private:
	UPROPERTY()
		class UMainGameInstance* gi;

	UPROPERTY()
		class AMainGameMode* MainGM;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		class UDataTable* CostPresetTable;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* CurrentSwordDamage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* NextSwordDamage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* SwordCost;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* CurrentGunDamage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* NextGunDamage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* HandgunCost;


	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnSwordUp;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnGunUp;

	FUpgradeCostTableData* GunCostData;
	FUpgradeCostTableData* SwordCostData;
};
