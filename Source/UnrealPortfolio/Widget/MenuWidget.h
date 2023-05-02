// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"
DECLARE_MULTICAST_DELEGATE(FDelegateMenuBtnFunc)
/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta = (BindWidget))
		class UButton* btnSave;

	UPROPERTY(meta = (BindWidget))
		class UButton* btnRetry;

	UPROPERTY(meta = (BindWidget))
		class UButton* btnGameQuit;

	UPROPERTY(meta = (BindWidget))
		class UButton* btnMenuClose;


	UPROPERTY()
		class UMainGameInstance* gi;

	UPROPERTY()
		class AMainGameMode* MainGM;

	UFUNCTION()
		void GameSave();

	UFUNCTION()
		void GameRetry();

	UFUNCTION()
		void GameQuit();

	UFUNCTION()
		void MenuClose();

	FDelegateMenuBtnFunc deleMenu;

	FORCEINLINE void SetIsShowedMenu(bool b) { isShowedMenu = b; }
private:
	bool isShowedMenu = false;
	bool isShowedConfirm = false;
};
