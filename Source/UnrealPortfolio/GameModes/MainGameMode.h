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

	void ShowPlayerHudWidget(); //������ ������ ��� Widget�� �����ϰ� �÷��̾� Hud�� ����

	void ShowAlertPopup(FString msg);

	void CloseAlertPopup();

	void PushTheEscButton();

	void DecreaseOrbs(FName orbNm, int32 val);

	void SetManaRatio();

	void SetHpRatio();

	void SetBossHpRatio(float r);

	void DoPlayerDeadAfter();

	//���� ���� ��ġ�� ����Ű�� �׺���̼��� ���
	void ShowNextLevelNavi();

	//���� ������ �޴� ���� ȣ��
	void ShowMenu();

	//�޴� ���� �ݱ�
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
		TSubclassOf<class UUserWidget> StoreWidgetClass; //���� ����

	UPROPERTY()
		TSubclassOf<class UUserWidget> AlertPopupWidgetClass; //��� �˾� ����

	UPROPERTY()
		TSubclassOf<class UUserWidget> MenuWidgetClass; //�޴� ����

	UPROPERTY()
		TSubclassOf<class UUserWidget> ConfirmWidgetClass; //Ȯ�� �˾� ����


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
