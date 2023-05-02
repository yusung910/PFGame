// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"

#include "Blueprint/UserWidget.h"
#include "Engine/LevelStreaming.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"

#include "PlayerCharacter.h"
#include "MainPlayerController.h"
#include "MainGameInstance.h"

#include "OrbsManagerComponent.h"

#include "DataSaveGame.h"
#include "MainWidget.h"
#include "StartGameMode.h"

AStartGameMode::AStartGameMode()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PlayerControllerClass = AMainPlayerController::StaticClass();

	//���� �÷��̾� ȭ���� widget�� ȣ���Ѵ�.
	GetClassAsset(IntroWidgetClass, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Widgets/BP_MainWidget.BP_MainWidget_C'");

}

void AStartGameMode::BeginPlay()
{
	//�θ� Ŭ������ BeginPlayer �Լ� ȣ��
	Super::BeginPlay();

	UGameplayStatics::SetGamePaused(GetWorld(), true);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;

	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), IntroWidgetClass);
	
	gi = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());

	//SaveGame Ŭ���� ����
	LoadData = Cast<UDataSaveGame>
		(UGameplayStatics::CreateSaveGameObject(UDataSaveGame::StaticClass()));

	IS_NULL(LoadData);

	//������ �ε�(���ϸ�, �ε��� ��ȣ)
	LoadData = Cast<UDataSaveGame>
		(UGameplayStatics::LoadGameFromSlot(LoadData->SaveSlotName, LoadData->SaveIndex));

	Cast<UMainWidget>(CurrentWidget)->SetLoadBtnVisiblity((LoadData != nullptr));

	CurrentWidget->AddToViewport();

}

void AStartGameMode::RemoveMainWidget()
{
	CurrentWidget->RemoveFromViewport();
}

void AStartGameMode::DoLoadData()
{
	IS_NULL(LoadData);
	gi->SetGameData(LoadData->Data);
	UGameplayStatics::OpenLevel(GetWorld(), FName(LoadData->Data.LevelNM));
}
