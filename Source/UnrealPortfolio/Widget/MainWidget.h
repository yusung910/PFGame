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
		void OnClickNewGame(); //���� ���� ��ư

	UFUNCTION()
		void OnClickLoad(); //���� �ε� ��ư

	UFUNCTION()
		void OnClickExit(); //���� ���� ��ư

	UFUNCTION()
		void SetLoadBtnVisiblity(bool flag);
};
