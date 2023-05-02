// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:

	UPROPERTY(meta = (BindWidget))
		class UButton* btnNewGame;

	UPROPERTY(meta = (BindWidget))
		class UButton* btnLoad;

	UPROPERTY(meta = (BindWidget))
		class UButton* btnExit;

	UPROPERTY()
		class UMainGameInstance* gi;

	UPROPERTY()
		class AStartGameMode* StartGM;

public:
	UFUNCTION()
		void OnClickNewGame(); //게임 시작 버튼

	UFUNCTION()
		void OnClickLoad(); //게임 로드 버튼

	UFUNCTION()
		void OnClickExit(); //게임 종료 버튼

	UFUNCTION()
		void SetLoadBtnVisiblity(bool flag);
};
