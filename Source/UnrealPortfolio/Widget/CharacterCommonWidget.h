// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterCommonWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UCharacterCommonWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateHealthBar(float ratio);

	void UpdateCastingBar(float ratio);

	void SetVisibleHpBar(bool flag);
	void SetVisibleCastingBar(bool flag);

private:
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* PB_HPBar;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* PB_CastingBar;
};
