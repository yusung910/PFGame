// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "OrbStructActor.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPlayerWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
		void InitOrbsWidget(UOrbsManagerComponent* comp);

	UFUNCTION()
		void InitOrbs();

	UFUNCTION()
		void UpdateOrbs();

	UFUNCTION()
		void ToggleStoreUseCopy(bool flag);

	UFUNCTION()
		void SetHpBar(float ratio);

	UFUNCTION()
		void SetManaBar(float ratio);

	UFUNCTION()
		void SetBossHpbar(float ratio);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UProgressBar* HpBar; //���� �÷��̾��� ü�� ������ ��(bar)

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UProgressBar* ManaBar; //���� �÷��̾��� ���� ������ ��(bar)

	//���� ������ ü�� ������ ��(bar)
	//���� ���Ͱ� ��ȯ �Ǿ��� ���� �����ش�.
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UProgressBar* BossHP; 

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UBorder* BgBorderOrbs;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UBorder* BgColorOrbs;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UVerticalBox* OrbsVerticalContainer;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UBorder* infoUseStore;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtEnemyCount;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtSwordLevel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtGunLevel;


	
private:
	UPROPERTY()
		class UMainGameInstance* gi;

	UPROPERTY()
		TArray<AActor*> EnemyList;

	UPROPERTY()
		TArray<AActor*> BossList;

	UPROPERTY()
		class UWidgetAnimation* FadeAnim;
	UPROPERTY()
		TSubclassOf<class UUserWidget> OrbsEntryAsset; //���� ȭ�� ����
	UPROPERTY()
		class UUserWidget* OrbsEntryWidget;
	UPROPERTY()
		class UOrbsManagerComponent* OrbsManagerComp;

	float fTickCnt = 1.0f;
	FTimerHandle WaitHandle;
};