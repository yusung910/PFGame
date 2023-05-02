// Fill out your copyright notice in the Description page of Project Settings.

#include "ConfirmWidget.h"

#include "MainGameInstance.h"
#include "MainGameMode.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"

void UConfirmWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	btnYes->OnClicked.AddDynamic(this, &UConfirmWidget::ConfirmYes);
}

void UConfirmWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->WasInputKeyJustReleased(EKeys::Escape))
	{
		MainGM->CloseAlertPopup();
	}
}

void UConfirmWidget::ConfirmYes()
{
	MainGM->ConfirmYes();
}