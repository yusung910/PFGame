// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/AlertPopupWidget.h"
#include "ConfirmWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UConfirmWidget : public UAlertPopupWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
		void ConfirmYes();

	UPROPERTY(meta = (BindWidget))
		class UButton* btnYes;
};
