// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "OrbStructActor.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UStoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UStoreWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
		void CloseStoreWidget();

	UFUNCTION()
		void InitOrbsWidget(UOrbsManagerComponent* comp);

	UFUNCTION()
		void ShowWeaponUpgradeList();

	UFUNCTION()
		void ShowStatusUpgradeList();

	UFUNCTION()
		void ShowETCList();
private:
	UPROPERTY()
		class UMainGameInstance* gi;

	UPROPERTY()
		class AMainGameMode* MainGM;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UBorder* BgBorderOrbs;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UBorder* BgColorOrbs;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnWeaponList;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnStatus;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnClose;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnEtc;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UVerticalBox* VB_UpgradeList;


	UPROPERTY()
		class UUserWidget* OrbsEntryWidget;

	UPROPERTY()
		TSubclassOf<class UUserWidget> OrbsEntryAsset;

	UPROPERTY()
		class UUserWidget* WeaponUpgradeWidget;

	UPROPERTY()
		TSubclassOf<class UUserWidget> WeaponUpgradeList;

	UPROPERTY()
		class UUserWidget* StatusUpgradeWidget;

	UPROPERTY()
		TSubclassOf<class UUserWidget> StatusUpgradeList;

	UPROPERTY()
		class UUserWidget* EtcWidget;

	UPROPERTY()
		TSubclassOf<class UUserWidget> EtcList;

	UPROPERTY()
		class UOrbsManagerComponent* OrbsManagerComp;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UVerticalBox* OrbsVerticalContainer;
};
