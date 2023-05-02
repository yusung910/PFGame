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

	//게임 플레이어 화면의 widget을 호출한다.
	GetClassAsset(IntroWidgetClass, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Widgets/BP_MainWidget.BP_MainWidget_C'");

}

void AStartGameMode::BeginPlay()
{
	//부모 클래스의 BeginPlayer 함수 호출
	Super::BeginPlay();

	UGameplayStatics::SetGamePaused(GetWorld(), true);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;

	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), IntroWidgetClass);
	
	gi = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());

	//SaveGame 클래스 생성
	LoadData = Cast<UDataSaveGame>
		(UGameplayStatics::CreateSaveGameObject(UDataSaveGame::StaticClass()));

	IS_NULL(LoadData);

	//데이터 로드(파일명, 인덱스 번호)
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
