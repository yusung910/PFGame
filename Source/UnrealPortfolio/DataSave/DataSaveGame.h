// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DataSaveGame.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FSaveData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		int32 OrbsAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		float GunLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		float SwordLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		float HpLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		float MaxHp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		float CurrentHP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		float ManaLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		float MaxMana;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		float CurrentMana;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		float HpHealBuyCnt;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		float ManaHealBuyCnt;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		float HpManaHealBuyCnt;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		float DmgGun;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		float DefaultDmgGun;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		float DmgSword;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		float DefaultDmgSword;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		FString LevelNM;

};

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UDataSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		FString SaveSlotName = TEXT("MainPlayer");	//데이터를 저장할 파일 이름

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		int32 SaveIndex = 0;	//저장할 파일 인덱스

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		FSaveData Data;	//저장할 구조체 정보

};
