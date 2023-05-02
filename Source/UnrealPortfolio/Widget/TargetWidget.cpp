// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"

#include "TargetWidget.h"
#include "Components/ProgressBar.h"


void UTargetWidget::SetHPBarRatio(float ratio)
{
	HpBar->SetPercent(ratio);
}