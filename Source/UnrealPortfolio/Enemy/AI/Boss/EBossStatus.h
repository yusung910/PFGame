// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EBossStatus.generated.h"

UENUM(BlueprintType)
enum class EBossStatusKey : uint8
{

	LeapAttack	UMETA(DisplayName = "LeapAttack"),
	NormAttack	UMETA(DisplayName = "NormAttack"),
	HadouKen	UMETA(DisplayName = "HadouKen"),
	Avoid		UMETA(DisplayName = "Avoid"),
	
};	//열거형 정의



UCLASS()
class UNREALPORTFOLIO_API AEBossStatus : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEBossStatus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
