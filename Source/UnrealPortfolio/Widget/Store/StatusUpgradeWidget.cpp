// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"
#include "StatusUpgradeWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetStringLibrary.h"

#include "MainGameMode.h"
#include "MainGameInstance.h"

UStatusUpgradeWidget::UStatusUpgradeWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//비용 관련 프리셋 테이블 데이터 호출
	GetObjectAsset(StatusCostList, UDataTable, "DataTable'/Game/DataTables/StatusCostList.StatusCostList'");
}

void UStatusUpgradeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	gi = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
	MainGM = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	//함수를 실행() 하는것이 아니라 함수의 주소값을 던져주는거임 함수 끝에 () 뺴야함
	btnHpUp->OnClicked.AddDynamic(this, &UStatusUpgradeWidget::HpLevelUp);
	btnManaUp->OnClicked.AddDynamic(this, &UStatusUpgradeWidget::ManaLevelUp);

	SetStatusUpgradeCostInfo();
}

void UStatusUpgradeWidget::HpLevelUp()
{
	if (HpUpgradeCostInfo)
	{
		if (gi->GetAmount() >= HpUpgradeCostInfo->CostHP)
		{
			MainGM->DecreaseOrbs("Red Orbs", HpUpgradeCostInfo->CostHP);
			gi->SetHpStatus(HpUpgradeCostInfo);
			SetStatusUpgradeCostInfo();
		}
		else
		{
			MainGM->ShowAlertPopup("Not Enough Orbs!");
		}
	}
	else
	{
		MainGM->ShowAlertPopup("Hp Level is Max!");
	}
}

void UStatusUpgradeWidget::ManaLevelUp()
{
	if (ManaUpgradeCostInfo)
	{
		if (gi->GetAmount() >= ManaUpgradeCostInfo->CostMana)
		{
			MainGM->DecreaseOrbs("Red Orbs", HpUpgradeCostInfo->CostMana);
			gi->SetManaStatus(HpUpgradeCostInfo);
			SetStatusUpgradeCostInfo();
		}
		else
		{
			MainGM->ShowAlertPopup("Not Enough Orbs!");
		}
	}
	else
	{
		MainGM->ShowAlertPopup("Mana Level is Max!");
	}
}

void UStatusUpgradeWidget::SetStatusUpgradeCostInfo()
{
	FString StrLocalHPLevel = FString::FromInt(gi->GetHpLevel() + 1);
	FString StrLocalManaLevel = FString::FromInt(gi->GetManaLevel() + 1);

	HpUpgradeCostInfo = StatusCostList->FindRow<FStatusUpgradeCostTableData>(FName(StrLocalHPLevel), TEXT("It is Not exist Row"));
	if (HpUpgradeCostInfo)
	{
		CurrentHP->SetText(UKismetTextLibrary::Conv_IntToText(HpUpgradeCostInfo->CurrentHP));
		NextHP->SetText(UKismetTextLibrary::Conv_IntToText(HpUpgradeCostInfo->NextHP));
		CostHP->SetText(UKismetTextLibrary::Conv_IntToText(HpUpgradeCostInfo->CostHP));
	}

	ManaUpgradeCostInfo = StatusCostList->FindRow<FStatusUpgradeCostTableData>(FName(StrLocalManaLevel), TEXT("It is Not exist Row"));

	if (ManaUpgradeCostInfo)
	{
		CurrentMana->SetText(UKismetTextLibrary::Conv_IntToText(HpUpgradeCostInfo->CurrentMana));
		NextMana->SetText(UKismetTextLibrary::Conv_IntToText(HpUpgradeCostInfo->NextMana));
		CostMana->SetText(UKismetTextLibrary::Conv_IntToText(HpUpgradeCostInfo->CostMana));
	}

}
