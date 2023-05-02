// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "DataSaveGame.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ECurrentWidgetStatusActor.h"

#include "FUpgradeCostTableData.h"

#include "MainGameInstance.generated.h"


DECLARE_MULTICAST_DELEGATE(FDeleLevelLoadDone)

UENUM(BlueprintType)
enum class EProgressReset : uint8
{
	UserHP		UMETA(DisplayName = "UserHP"),
	UserMana	UMETA(DisplayName = "UserMana"),
	BossHP		UMETA(DisplayName = "BossHP"),

};	//열거형 정의

/**
 *
 */
UCLASS()
class UNREALPORTFOLIO_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMainGameInstance();

	virtual void Init() override;

	void SetHpStatus(FStatusUpgradeCostTableData* info);
	void SetManaStatus(FStatusUpgradeCostTableData* info);

	void HealStatus(int32 type);

	void RecoveryHpMana(FName Type);

	void SetWeaponDamage(bool isUltimate);

	void GameQuit();

	void FullRecovery();

	FSaveData GetGameData();

	void SetGameData(FSaveData data);

	FDeleLevelLoadDone FDeleLevelOpen;

	FORCEINLINE void SetAmount(int32 amount) { OrbsAmount = amount; }
	FORCEINLINE int32 GetAmount() { return OrbsAmount; }

	FORCEINLINE float GetGunLevel() { return GunLevel; }
	FORCEINLINE void GunLevelUp() { GunLevel++; }

	FORCEINLINE float GetSwordLevel() { return SwordLevel; }
	FORCEINLINE void SwordLevelUp() { SwordLevel++; }

	FORCEINLINE float GetHpLevel() { return HpLevel; }
	FORCEINLINE float GetManaLevel() { return ManaLevel; }

	FORCEINLINE void AccumulateCurrentHp(float n) { CurrentHP += n; }
	FORCEINLINE void AccumulateCurrentMana(float n) { CurrentMana += n; }

	FORCEINLINE float GetCurrentHp() { return CurrentHP; }
	FORCEINLINE float GetCurrentMana() { return CurrentMana; }

	FORCEINLINE void IncreasedHpHealBuyCnt() { HpHealBuyCnt++; }
	FORCEINLINE float GetHpHealBuyCnt() { return HpHealBuyCnt; }

	FORCEINLINE void IncreasedManaHealBuyCnt() { ManaHealBuyCnt++; }
	FORCEINLINE float GetManaHealBuyCnt() { return ManaHealBuyCnt; }

	FORCEINLINE void IncreasedHpManaHealBuyCnt() { HpManaHealBuyCnt++; }
	FORCEINLINE float GetHpManaHealBuyCnt() { return HpManaHealBuyCnt; }

	FORCEINLINE float GetDmgGun() { return DmgGun; }
	FORCEINLINE void SetDmgGun(float val) { DmgGun = val; }
	FORCEINLINE void SetDefaultDmgGun(float val) { DefaultDmgGun = val; }

	FORCEINLINE float GetDmgSword() { return DmgSword; }
	FORCEINLINE void SetDmgSword(float val) { DmgSword = val; }
	FORCEINLINE void SetDefaultDmgSword(float val) { DefaultDmgSword = val; }

	FORCEINLINE float GetMontagePlaySpeed() { return MontagePlaySpeed; }

	FORCEINLINE void SetMonsterCount(float val) { fEnemyCount = val; }
	FORCEINLINE float GetMonsterCount() { return fEnemyCount; }

	FORCEINLINE float GetHpRatio() { return CurrentHP / MaxHp; }
	FORCEINLINE float GetManaRatio() { return CurrentMana / MaxMana; }

private: //User Status
	//게임 내의 재화 수치
	int32 OrbsAmount = 1000;

	float GunLevel = 1.f;
	float SwordLevel = 1.f;

	float HpLevel = 0.f;
	float MaxHp = 100.f; //최대 생명력
	float CurrentHP = 100.f; //현재 생명력(데미지 피격시 사용됨)

	float ManaLevel = 0.f;
	float MaxMana = 50.f; //최대 마나
	float CurrentMana = 50.f; //현재 마나(마나 사용시 소모됨)

	float HpHealBuyCnt = 1;
	float ManaHealBuyCnt = 1;
	float HpManaHealBuyCnt = 1;

	float DmgGun = 5.f;
	float DefaultDmgGun = 5.0f;
	float DmgSword = 10.f;
	float DefaultDmgSword = 10.f;

	float UltimateIncreaseRate = 1.2f;

	float MontagePlaySpeed = 1.2f;

	float fEnemyCount = 0.f;

	UPROPERTY()
		TArray<int32> PlayerOrbsCurrent = { 0, 0, 0 };

};
