// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TargetWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UTargetWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHPBarRatio(float ratio);

private:
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HpBar;
	
};
