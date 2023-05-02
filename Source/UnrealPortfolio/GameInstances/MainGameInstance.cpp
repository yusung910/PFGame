// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"

#include "EnemyCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "GameFramework/PlayerStart.h"
#include "GameFramework/GameMode.h"

#include "UObject/UObjectGlobals.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "OrbsManagerComponent.h"
#include "PlayerCharacter.h"
#include "AlertPopupWidget.h"

#include "PlayerWidget.h"

#include "MainGameMode.h"

#include "DataSaveGame.h"

#include "MainGameInstance.h"



UMainGameInstance::UMainGameInstance()
{
}

void UMainGameInstance::Init()
{
	Super::Init();

	if (FDeleLevelOpen.IsBound())
	{
		FDeleLevelOpen.Broadcast();
	}
}


void UMainGameInstance::SetHpStatus(FStatusUpgradeCostTableData* info)
{
	HpLevel = info->StatusLevel;
	MaxHp = info->CurrentHP;
	CurrentHP = info->CurrentHP;
}

void UMainGameInstance::SetManaStatus(FStatusUpgradeCostTableData* info)
{
	ManaLevel = info->StatusLevel;
	MaxMana = info->CurrentMana;
	CurrentMana = info->CurrentMana;
}

void UMainGameInstance::HealStatus(int32 type)
{
	switch (type)
	{
	case 0:
		CurrentHP = MaxHp;
		break;
	case 1:
		CurrentMana = MaxMana;
		break;
	case 2:
		CurrentHP = MaxHp;
		CurrentMana = MaxMana;
		break;
	}
	//캐릭터의 옷 색상을 원래대로 변환
	Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->SetClothMaterialByHp(false);
}

void UMainGameInstance::RecoveryHpMana(FName Type)
{
	if (Type.IsEqual("green"))
	{
		float r = UKismetMathLibrary::RandomFloatInRange(0, 10);
		CurrentHP = UKismetMathLibrary::FClamp(CurrentHP + r, 0, MaxHp);
	}
	else if (Type.IsEqual("blue"))
	{
		float r = UKismetMathLibrary::RandomFloatInRange(0, 10);
		CurrentMana = UKismetMathLibrary::FClamp(CurrentMana + r, 0, MaxMana);
	}

	AMainGameMode* md = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	IS_NULL(md)
	md->SetHpRatio();
	md->SetManaRatio();

}

void UMainGameInstance::SetWeaponDamage(bool isUltimate)
{
	if (isUltimate)
	{
		DmgGun = DefaultDmgGun + (DefaultDmgGun * UltimateIncreaseRate);
		DmgSword = DefaultDmgSword + (DefaultDmgSword * UltimateIncreaseRate);
		MontagePlaySpeed = 1.4f;
	}
	else
	{
		DmgGun = DefaultDmgGun;
		DefaultDmgSword = DefaultDmgSword;
		MontagePlaySpeed = 1.2f;
	}
}

void UMainGameInstance::GameQuit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}

void UMainGameInstance::FullRecovery()
{
	CurrentHP = MaxHp;
	CurrentMana = MaxMana;

	AMainGameMode* md = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	IS_NULL(md)
		md->SetHpRatio();
	md->SetManaRatio();
}

FSaveData UMainGameInstance::GetGameData()
{
	FSaveData retData;
	retData.OrbsAmount = OrbsAmount;
	retData.GunLevel = GunLevel;
	retData.SwordLevel = SwordLevel;
	retData.HpLevel = HpLevel;
	retData.MaxHp = MaxHp;
	retData.CurrentHP = CurrentHP;
	retData.ManaLevel = ManaLevel;
	retData.MaxMana = MaxMana;
	retData.CurrentMana = CurrentMana;
	retData.HpHealBuyCnt = HpHealBuyCnt;
	retData.ManaHealBuyCnt = ManaHealBuyCnt;
	retData.HpManaHealBuyCnt = HpManaHealBuyCnt;
	retData.DmgGun = DmgGun;
	retData.DefaultDmgGun = DefaultDmgGun;
	retData.DmgSword = DmgSword;
	retData.DefaultDmgSword = DefaultDmgSword;
	retData.LevelNM = "Dungeon_1";

	return retData;
}

void UMainGameInstance::SetGameData(FSaveData data)
{
	OrbsAmount = data.OrbsAmount;
	GunLevel = data.GunLevel;
	SwordLevel = data.SwordLevel;
	HpLevel = data.HpLevel;
	MaxHp = data.MaxHp;
	CurrentHP = data.CurrentHP;
	ManaLevel = data.ManaLevel;
	MaxMana = data.MaxMana;
	CurrentMana = data.CurrentMana;
	HpHealBuyCnt = data.HpHealBuyCnt;
	ManaHealBuyCnt = data.ManaHealBuyCnt;
	HpManaHealBuyCnt = data.HpManaHealBuyCnt;
	DmgGun = data.DmgGun;
	DefaultDmgGun = data.DefaultDmgGun;
	DmgSword = data.DmgSword;
	DefaultDmgSword = data.DefaultDmgSword;
}

