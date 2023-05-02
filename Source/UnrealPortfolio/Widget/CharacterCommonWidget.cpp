// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCommonWidget.h"

#include "Components/ProgressBar.h"

void UCharacterCommonWidget::UpdateHealthBar(float ratio)
{
	if (PB_HPBar == nullptr) return;
	PB_HPBar->SetPercent(ratio);
}

void UCharacterCommonWidget::UpdateCastingBar(float ratio)
{
	if (PB_CastingBar == nullptr) return;
	PB_CastingBar->SetPercent(ratio);
}

void UCharacterCommonWidget::SetVisibleHpBar(bool flag)
{
	ESlateVisibility visibleFlag = (flag) ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	PB_HPBar->SetVisibility(visibleFlag);
}

void UCharacterCommonWidget::SetVisibleCastingBar(bool flag)
{
	ESlateVisibility visibleFlag = (flag) ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	PB_CastingBar->SetVisibility(visibleFlag);
}
