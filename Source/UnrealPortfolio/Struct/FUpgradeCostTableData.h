// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/DataTable.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FUpgradeCostTableData.generated.h"

//업그레이드 비용 테이블
USTRUCT(Atomic, BlueprintType)
struct FUpgradeCostTableData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WeaponLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentSwordDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NextSwordDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SwordCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentGunDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NextGunDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HandgunCost;
};


USTRUCT(Atomic, BlueprintType)
struct FStatusUpgradeCostTableData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StatusLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NextHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CostHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NextMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CostMana;
};



UCLASS()
class UNREALPORTFOLIO_API AFUpgradeCostTableData : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFUpgradeCostTableData();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
