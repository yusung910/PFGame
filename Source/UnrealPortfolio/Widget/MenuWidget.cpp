// Fill out your copyright notice in the Description page of Project Settings.
#include "UnrealPortfolio.h"

#include "MainGameInstance.h"
#include "MainGameMode.h"
#include "Components/Button.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"

#include "MenuWidget.h"

void UMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	gi = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
	MainGM = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//함수를 실행() 하는것이 아니라 함수의 주소값을 던져주는거임 함수 끝에 () 뺴야함
	btnSave->OnClicked.AddDynamic(this, &UMenuWidget::GameSave);
	btnRetry->OnClicked.AddDynamic(this, &UMenuWidget::GameRetry);
	btnGameQuit->OnClicked.AddDynamic(this, &UMenuWidget::GameQuit);
	btnMenuClose->OnClicked.AddDynamic(this, &UMenuWidget::MenuClose);

}

void UMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->WasInputKeyJustReleased(EKeys::Escape) && isShowedMenu)
	{
		MenuClose();
	}

	if (deleMenu.IsBound())
	{
		deleMenu.Broadcast();
	}
}


void UMenuWidget::GameSave()
{
	MainGM->GameProgressDataSave();
	MainGM->ShowAlertPopup("Save Complete!");
	MainGM->delBtnFun.Clear();
	MainGM->delBtnFun.AddLambda([this] {
		MainGM->ShowMenu();
	});
}

void UMenuWidget::GameRetry()
{
	MainGM->ShowConfirmAlert("Do you want to Retry Game?");
	MainGM->delBtnFun.Clear();
	MainGM->delBtnFun.AddLambda([this] {
		MainGM->ShowMenu();
	});

	MainGM->deleConfrimYes.Clear();
	MainGM->deleConfrimYes.AddLambda([this] {
		FString levelNm = UGameplayStatics::GetCurrentLevelName(GetWorld());
		UGameplayStatics::OpenLevel(this, FName(levelNm));
	});

}

void UMenuWidget::GameQuit()
{
	MainGM->ShowConfirmAlert("Do you want to Quit Game?");
	MainGM->delBtnFun.Clear();
	MainGM->delBtnFun.AddLambda([this] {
		MainGM->ShowMenu();
	});

	MainGM->deleConfrimYes.Clear();
	MainGM->deleConfrimYes.AddLambda([this] {
		UKismetSystemLibrary::QuitGame(this, 0, EQuitPreference::Quit, false);
	});
}

void UMenuWidget::MenuClose()
{
	isShowedMenu = false;
	MainGM->CloseMenu();
}


