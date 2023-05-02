// Fill out your copyright notice in the Description page of Project Settings.


#include "ETCListWidget.h"

#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetStringLibrary.h"

#include "MainGameMode.h"
#include "MainGameInstance.h"

void UETCListWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	gi = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
	MainGM = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	//함수를 실행() 하는것이 아니라 함수의 주소값을 던져주는거임 함수 끝에 () 뺴야함
	btnHealHp->OnClicked.AddDynamic(this, &UETCListWidget::BuyHealHp);
	btnHealMana->OnClicked.AddDynamic(this, &UETCListWidget::BuyHealMana);
	btnHPManaHeal->OnClicked.AddDynamic(this, &UETCListWidget::BuyHealHpMana);

	HealItemCostSet();
}

void UETCListWidget::HealItemCostSet()
{
	HpHealCostTxt->SetText(UKismetTextLibrary::Conv_FloatToText((HpHealCost * gi->GetHpHealBuyCnt()), ERoundingMode::ToZero));
	ManaHealCostTxt->SetText(UKismetTextLibrary::Conv_FloatToText(ManaHealCost * gi->GetManaHealBuyCnt(), ERoundingMode::ToZero));
	HpManaHealCostTxt->SetText(UKismetTextLibrary::Conv_FloatToText(HpManaHealCost * gi->GetHpManaHealBuyCnt(), ERoundingMode::ToZero));
}

void UETCListWidget::BuyHealHp()
{
	float LocalHpHealCost = HpHealCost * gi->GetHpHealBuyCnt();
	if (gi->GetAmount() >= LocalHpHealCost)
	{
		gi->IncreasedHpHealBuyCnt();
		MainGM->DecreaseOrbs("Red Orbs", LocalHpHealCost);
		gi->HealStatus(0);
		HealItemCostSet();
	}
	else
	{
		MainGM->ShowAlertPopup("Not Enough Orbs!");
	}
}

void UETCListWidget::BuyHealMana()
{
	float LocalManaHealCost = ManaHealCost * gi->GetManaHealBuyCnt();
	if (gi->GetAmount() >= LocalManaHealCost)
	{
		gi->IncreasedManaHealBuyCnt();
		MainGM->DecreaseOrbs("Red Orbs", LocalManaHealCost);
		gi->HealStatus(1);
		HealItemCostSet();
	}
	else
	{
		MainGM->ShowAlertPopup("Not Enough Orbs!");
	}

}

void UETCListWidget::BuyHealHpMana()
{
	float LocalHpManaHealCost = HpManaHealCost * gi->GetManaHealBuyCnt();
	if (gi->GetAmount() >= LocalHpManaHealCost)
	{
		gi->IncreasedHpManaHealBuyCnt();
		MainGM->DecreaseOrbs("Red Orbs", LocalHpManaHealCost);
		gi->HealStatus(2);
		HealItemCostSet();
	}
	else
	{
		MainGM->ShowAlertPopup("Not Enough Orbs!");
	}
}
