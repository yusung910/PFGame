// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AlertPopupWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UAlertPopupWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* TxtContent;

	UPROPERTY(meta = (BindWidget))
		class UButton* btnCheck;

	void SetMessageCopy(FString message);

	UFUNCTION()
		void CloseAlertPopup();

	UPROPERTY()
		class UMainGameInstance* gi;

	UPROPERTY()
		class AMainGameMode* MainGM;
};
