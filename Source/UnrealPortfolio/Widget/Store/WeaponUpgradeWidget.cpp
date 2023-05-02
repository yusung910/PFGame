// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"
#include "WeaponUpgradeWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "MainGameMode.h"
#include "MainGameInstance.h"


UWeaponUpgradeWidget::UWeaponUpgradeWidget(const FObjectInitializer& ObjectInitializer) : Super (ObjectInitializer)
{
	//비용 관련 프리셋 테이블 데이터 호출
	GetObjectAsset(CostPresetTable, UDataTable, "DataTable'/Game/DataTables/upgradeCost.upgradeCost'");
}

void UWeaponUpgradeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	gi = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
	MainGM = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//함수를 실행() 하는것이 아니라 함수의 주소값을 던져주는거임 함수 끝에 () 뺴야함
	btnSwordUp->OnClicked.AddDynamic(this, &UWeaponUpgradeWidget::SwordLevelUp);
	btnGunUp->OnClicked.AddDynamic(this, &UWeaponUpgradeWidget::GunLevelUp);

	SetWeaponUpgradeCostInfo();
}

void UWeaponUpgradeWidget::SetWeaponUpgradeCostInfo()
{
	FString LocalGunLevelStr = FString::FromInt(gi->GetGunLevel());

	GunCostData = CostPresetTable->FindRow<FUpgradeCostTableData>(FName(LocalGunLevelStr), TEXT("It is Not exist Row"));
	if (GunCostData)
	{
		FText LocalGunCost = UKismetTextLibrary::Conv_IntToText(GunCostData->HandgunCost);
		FText LocalGunCurrentDmg = UKismetTextLibrary::Conv_IntToText(GunCostData->CurrentGunDamage);
		FText LocalGunNextDmg = UKismetTextLibrary::Conv_IntToText(GunCostData->NextGunDamage);

		CurrentGunDamage->SetText(LocalGunCurrentDmg);
		NextGunDamage->SetText(LocalGunNextDmg);
		HandgunCost->SetText(LocalGunCost);
	}

	FString LocalSwordLevelStr = FString::FromInt(gi->GetSwordLevel());

	SwordCostData = CostPresetTable->FindRow<FUpgradeCostTableData>(FName(LocalSwordLevelStr), TEXT("It is Not exist Row"));
	if (SwordCostData)
	{
		FText LocalSwordCost = UKismetTextLibrary::Conv_IntToText(SwordCostData->SwordCost);
		FText LocalSwordCurrentDmg = UKismetTextLibrary::Conv_IntToText(SwordCostData->CurrentSwordDamage);
		FText LocalSwordNextDmg = UKismetTextLibrary::Conv_IntToText(SwordCostData->NextSwordDamage);

		CurrentSwordDamage->SetText(LocalSwordCurrentDmg);
		NextSwordDamage->SetText(LocalSwordNextDmg);
		SwordCost->SetText(LocalSwordCost);
	}
}

void UWeaponUpgradeWidget::SwordLevelUp()
{
	//orb 지불 후 레벨업 기능 추가
	//레벨 10제한 추가
	if (SwordCostData)
	{
		if (gi->GetAmount() >= SwordCostData->SwordCost)
		{
			gi->SwordLevelUp();
			gi->SetDefaultDmgSword(SwordCostData->CurrentSwordDamage);
			MainGM->DecreaseOrbs("Red Orbs", SwordCostData->SwordCost);
			SetWeaponUpgradeCostInfo();
		}
		else
		{
			MainGM->ShowAlertPopup("Not Enough Orbs!");
		}
	}
	else
	{
		MainGM->ShowAlertPopup("Sword Level is Max!");
	}
}

void UWeaponUpgradeWidget::GunLevelUp()
{
	if (GunCostData)
	{
		if (gi->GetAmount() >= GunCostData->HandgunCost)
		{
			gi->GunLevelUp();
			MainGM->DecreaseOrbs("Red Orbs", GunCostData->HandgunCost);
			gi->SetDefaultDmgGun(SwordCostData->CurrentGunDamage);
			SetWeaponUpgradeCostInfo();
		}
		else
		{
			MainGM->ShowAlertPopup("Not Enough Orbs!");
		}
	}
	else
	{
		MainGM->ShowAlertPopup("Gun Level is Max!");
	}
}
