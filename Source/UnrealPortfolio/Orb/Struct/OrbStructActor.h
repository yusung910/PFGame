// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OrbEnumActor.h"

#include "Engine/DataTable.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "OrbStructActor.generated.h"


USTRUCT(Atomic, BlueprintType)
struct FOrbsReceiveData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* SenderActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* SpawnerActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* OrbActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EOrbScale Scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SendOption;
};


// * 구조체를 블루프린트에서 사용하기
// Struct는 접미어로 F를 붙여야함
USTRUCT(Atomic, BlueprintType)
struct FOrbsSendData
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Type = "None";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Amount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EOrbScale Scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<float> ScalePercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SendOption;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bInfinitySend;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDynamicDecrease;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AOrbsSpawnerActor* SpawnerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AOrbsProjectilesBaseActor* ProjectilesClass;

};

USTRUCT(Atomic, BlueprintType)
struct FOrbsSpawnerData
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EOrbScale Scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Options;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class  AOrbsSpawnerActor* SpawnerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AOrbsProjectilesBaseActor* ProjectilesClass;
};

USTRUCT(Atomic, BlueprintType)
struct FOrbsTableData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName OrbsName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor OrbsColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString OrbsOption;

};

USTRUCT(Atomic, BlueprintType)
struct FOrbsDynamicData
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName OrbsName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor OrbsColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString OrbsOption;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CurrentAmount;


};



UCLASS()
class UNREALPORTFOLIO_API AOrbStructActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AOrbStructActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
