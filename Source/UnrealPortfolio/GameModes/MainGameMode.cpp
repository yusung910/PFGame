// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "GameFramework/GameMode.h"

#include "PlayerCharacter.h"

#include "WarfareTriggerVolActor.h"
#include "NavigationLaserActor.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "GameFramework/PlayerStart.h"
#include "MainPlayerController.h"
#include "OrbsManagerComponent.h"

#include "StoreWidget.h"
#include "PlayerWidget.h"
#include "MenuWidget.h"
#include "AlertPopupWidget.h"
#include "ConfirmWidget.h"
#include "DataSaveGame.h"

#include "MainGameInstance.h"

#include "MainGameMode.h"

AMainGameMode::AMainGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PlayerControllerClass = AMainPlayerController::StaticClass();

	//���� �÷��̾� ȭ���� widget�� ȣ���Ѵ�.
	GetClassAsset(PlayerHudWidgetClass, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Widgets/BP_PlayerWidget.BP_PlayerWidget_C'");

	//���� ���� ȣ��
	GetClassAsset(StoreWidgetClass, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Widgets/Store/WB_Store.WB_Store_C'");

	//�� �˾� ȣ��
	GetClassAsset(AlertPopupWidgetClass, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Widgets/WB_AlertPopup.WB_AlertPopup_C'");

	//�޴� �˾� ȣ�� 
	GetClassAsset(MenuWidgetClass, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Widgets/WB_Menu.WB_Menu_C'");

	//
	GetClassAsset(ConfirmWidgetClass, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Widgets/WB_ConfirmPopup.WB_ConfirmPopup_C'");

	
}


void AMainGameMode::BeginPlay()
{
	//�θ� Ŭ������ BeginPlayer �Լ� ȣ��
	Super::BeginPlay();
	
	//���� ���忡 �ִ� 0��° �÷��̾��� controller ȣ���Ͽ� bShowMouseCursor���� true�� �ٲ۴�
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;
	IS_NULL(PlayerHudWidgetClass);
	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHudWidgetClass); //���� ����
	CurrentWidget->AddToViewport(); //����Ʈ�� �߰�

	OrbsComponent = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetOrbsComponent();
	
	gi = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());

	nextWarp = UGameplayStatics::GetActorOfClass(GetWorld(), AWarfareTriggerVolActor::StaticClass());

	IS_NULL(nextWarp);
	nextWarp->SetHidden(true);
}

void AMainGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Enemy"), EnemyList);
	if (EnemyList.Num() > 0)
	{
		isLevelClear = false;
	}
	else if (EnemyList.Num() == 0 && !isLevelClear)
	{
		isLevelClear = true;
		ShowNextLevelNavi();
	}
}

void AMainGameMode::ShowStoreWidget()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
	CurrentWidget->RemoveFromViewport();
	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), StoreWidgetClass); //���� ����
	CurrentWidget->AddToViewport(); //����Ʈ�� �߰�

	gi->SetAmount(OrbsComponent->GetOrbsDatas()[0].CurrentAmount);

	isUsingStore = true;
}


void AMainGameMode::ShowPlayerHudWidget()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;
	CurrentWidget->RemoveFromViewport();
	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHudWidgetClass); //���� ����
	CurrentWidget->AddToViewport(); //����Ʈ�� �߰�

	isUsingStore = false;

	IS_NULL(OrbsComponent);
	Cast<UPlayerWidget>(CurrentWidget)->InitOrbsWidget(OrbsComponent);
}

void AMainGameMode::ShowAlertPopup(FString msg)
{
	if (MenuWidget != nullptr)
	{
		MenuWidget->RemoveFromViewport();
	}
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;

	AlertWidget = CreateWidget<UUserWidget>(GetWorld(), AlertPopupWidgetClass); //���� ����
	AlertWidget->AddToViewport();
	Cast<UAlertPopupWidget>(AlertWidget)->SetMessageCopy(msg);
	isShowAlert = true;
}

void AMainGameMode::CloseAlertPopup()
{
	
	if (AlertWidget != nullptr)
	{
		AlertWidget->RemoveFromViewport();
		isShowAlert = false;
	}

	if (ConfirmWidget != nullptr)
	{
		ConfirmWidget->RemoveFromViewport();
	}
	
	if (delBtnFun.IsBound() == true)
	{
		delBtnFun.Broadcast();
		delBtnFun.Clear();
	}
}

void AMainGameMode::PushTheEscButton()
{
	if (isShowAlert && isUsingStore)
	{
		AlertWidget->RemoveFromViewport();
		isShowAlert = false;
	}
	else if(!isShowAlert && isUsingStore)
	{
		IS_NULL(CurrentWidget);
		CurrentWidget->RemoveFromViewport();
		ShowPlayerHudWidget();
	}

}

void AMainGameMode::DecreaseOrbs(FName orbNm, int32 val)
{
	OrbsComponent->DecreaseOrbs(orbNm, val);
	OrbsComponent->OrbsSpawnWidgetInStore();
}

void AMainGameMode::SetManaRatio()
{
	IS_NULL(CurrentWidget);
	UPlayerWidget* playerHud = Cast<UPlayerWidget>(CurrentWidget);
	IS_NULL(playerHud);
	playerHud->SetManaBar(gi->GetManaRatio());
}

void AMainGameMode::SetHpRatio()
{
	IS_NULL(CurrentWidget);
	UPlayerWidget* playerHud = Cast<UPlayerWidget>(CurrentWidget);
	IS_NULL(playerHud);
	playerHud->SetHpBar(gi->GetHpRatio());
}

void AMainGameMode::SetBossHpRatio(float r)
{
	UPlayerWidget* playerHud = Cast<UPlayerWidget>(CurrentWidget);
	IS_NULL(playerHud);
	playerHud->SetBossHpbar(r);
}

void AMainGameMode::DoPlayerDeadAfter()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Dungeon_1"));

	gi->FullRecovery();
}

void AMainGameMode::ShowNextLevelNavi()
{
	IS_NULL(nextWarp);

	UNavigationPath* tpath;
	UNavigationSystemV1* LocalNavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	nextWarp->SetHidden(false);

	AActor* Starting = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());

	tpath = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), Starting->GetActorLocation(), nextWarp->GetActorLocation());

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANavigationLaserActor::StaticClass(), NavigationActorList);
	
	if (NavigationActorList.Num() > 0)
	{
		for (auto actor : NavigationActorList)
		{
			actor->Destroy();
		}
	}

	if (tpath != NULL)
	{
		for (int pointiter = 1; pointiter < tpath->PathPoints.Num(); pointiter++)
		{

			FTransform transform;
			transform.SetLocation(tpath->PathPoints[pointiter-1]);

			ANavigationLaserActor* naviPoint = GetWorld()->SpawnActor<ANavigationLaserActor>(ANavigationLaserActor::StaticClass(), transform);

			NavigationActorList.Add(naviPoint);

			naviPoint->SetLaserPointDirection(tpath->PathPoints[pointiter - 1], tpath->PathPoints[pointiter]);

		}
	}
}

void AMainGameMode::ShowMenu()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
	MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass); //���� ����
	MenuWidget->AddToViewport(); //����Ʈ�� �߰�

	UMenuWidget* menuWidget = Cast<UMenuWidget>(MenuWidget);
	menuWidget->deleMenu.AddLambda([this] {
		ShowMenuDone();
	});
}



void AMainGameMode::CloseMenu()
{
	IS_NULL(MenuWidget);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;
	
	MenuWidget->RemoveFromViewport(); //����Ʈ���� ����
}

void AMainGameMode::ShowMenuDone()
{
	IS_NULL(MenuWidget);
	UMenuWidget* LocalMenuWidget = Cast<UMenuWidget>(MenuWidget);
	LocalMenuWidget->SetIsShowedMenu(true);
}

void AMainGameMode::ShowConfirmAlert(FString msg)
{
	if (MenuWidget != nullptr)
	{
		MenuWidget->RemoveFromViewport();
	}
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
	ConfirmWidget = CreateWidget<UUserWidget>(GetWorld(), ConfirmWidgetClass); //���� ����
	ConfirmWidget->AddToViewport();
	Cast<UConfirmWidget>(ConfirmWidget)->SetMessageCopy(msg);
}

void AMainGameMode::CloseConfirmAlert()
{
	IS_NULL(ConfirmWidget);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;

	ConfirmWidget->RemoveFromViewport(); //����Ʈ���� ����

	if (deleCloseConfirm.IsBound())
	{
		deleCloseConfirm.Broadcast();
	}
}

void AMainGameMode::ConfirmYes()
{
	if (deleConfrimYes.IsBound())
	{
		deleConfrimYes.Broadcast();
	}
}

void AMainGameMode::GameProgressDataSave()
{
	//SaveGame Ŭ���� ����
	UDataSaveGame* SaveData = Cast<UDataSaveGame>
		(UGameplayStatics::CreateSaveGameObject(UDataSaveGame::StaticClass()));

	IS_NULL(SaveData);
	SaveData->Data = gi->GetGameData();

	//������ ����(������ SaveGameŬ����, ���ϸ�, �ε�����ȣ)
	UGameplayStatics::SaveGameToSlot(SaveData, SaveData->SaveSlotName, SaveData->SaveIndex);
}

void AMainGameMode::ShowMainLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Main"));
}
