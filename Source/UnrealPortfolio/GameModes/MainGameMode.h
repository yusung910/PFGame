// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE(FDelegateAlertBtnFunc)
DECLARE_MULTICAST_DELEGATE(FDelConfirmPopupCalled)
DECLARE_MULTICAST_DELEGATE(FDeleConfirmClose)
DECLARE_MULTICAST_DELEGATE(FDeleYesConfirm)
/**
 *
 */
UCLASS()
class UNREALPORTFOLIO_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMainGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void ShowStoreWidget();

	void ShowPlayerHudWidget(); //기존에 생성된 모든 Widget을 제거하고 플레이어 Hud를 생성

	void ShowAlertPopup(FString msg);

	void CloseAlertPopup();

	void PushTheEscButton();

	void DecreaseOrbs(FName orbNm, int32 val);

	void SetManaRatio();

	void SetHpRatio();

	void SetBossHpRatio(float r);

	void DoPlayerDeadAfter();

	//다음 레벨 위치를 가리키는 네비게이션을 출력
	void ShowNextLevelNavi();

	//게임 진행중 메뉴 위젯 호출
	void ShowMenu();

	//메뉴 위젯 닫기
	void CloseMenu();

	void ShowMenuDone();

	void ShowConfirmAlert(FString msg);

	void CloseConfirmAlert();

	
	void ConfirmYes();

	void GameProgressDataSave();

	void ShowMainLevel();
	FDelegateAlertBtnFunc delBtnFun;
	FDelConfirmPopupCalled delConfirm;

	FDeleConfirmClose deleCloseConfirm;
	FDeleYesConfirm deleConfrimYes;

	FORCEINLINE bool GetIsLevelClear() { return isLevelClear; }
	FORCEINLINE void SetIsLevelClear(bool b) { isLevelClear = b; }

private:
	UPROPERTY()
		TSubclassOf<class UUserWidget> PlayerHudWidgetClass;

	UPROPERTY()
		TSubclassOf<class UUserWidget> StoreWidgetClass; //상점 위젯

	UPROPERTY()
		TSubclassOf<class UUserWidget> AlertPopupWidgetClass; //경고 팝업 위젯

	UPROPERTY()
		TSubclassOf<class UUserWidget> MenuWidgetClass; //메뉴 위젯

	UPROPERTY()
		TSubclassOf<class UUserWidget> ConfirmWidgetClass; //확인 팝업 위젯


	UPROPERTY()
		class UUserWidget* CurrentWidget;

	UPROPERTY()
		class UUserWidget* AlertWidget;

	UPROPERTY()
		class UUserWidget* MenuWidget;

	UPROPERTY()
		class UUserWidget* StoreWidget;

	UPROPERTY()
		class UUserWidget* ConfirmWidget;

	UPROPERTY()
		TArray<AActor*> EnemyList;

	UPROPERTY()
		class UOrbsManagerComponent* OrbsComponent;

	UPROPERTY()
		class UMainGameInstance* gi;

	UPROPERTY()
		TArray<class AActor*> NavigationActorList;

	UPROPERTY()
		class AActor* nextWarp;

	bool isUsingStore = false;
	bool isShowAlert = false;

	bool isLevelClear = false;
};
