// Fill out your copyright notice in the Description page of Project Settings.
#include "AlertPopupWidget.h"

#include "MainGameInstance.h"
#include "MainGameMode.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"

void UAlertPopupWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	gi = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
	MainGM = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	btnCheck->OnClicked.AddDynamic(this, &UAlertPopupWidget::CloseAlertPopup);
}

void UAlertPopupWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->WasInputKeyJustReleased(EKeys::Escape))
	{
		MainGM->CloseAlertPopup();
	}
}

void UAlertPopupWidget::SetMessageCopy(FString message)
{
	TxtContent->SetText(UKismetTextLibrary::Conv_StringToText(message));
}

void UAlertPopupWidget::CloseAlertPopup()
{
	MainGM->CloseAlertPopup();
}
