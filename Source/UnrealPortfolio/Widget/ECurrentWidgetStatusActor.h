// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ECurrentWidgetStatusActor.generated.h"

UENUM(BlueprintType)
enum class ECurrentWidgetStatus : uint8
{
	INTRO		UMETA(DisplayName = "INTRO"),
	ALERT		UMETA(DisplayName = "ALERT"),
	PLAYER_HUD	UMETA(DisplayName = "PLAYER_HUD"),
	STORE		UMETA(DisplayName = "STORE"),
};	//열거형 정의

UCLASS()
class UNREALPORTFOLIO_API AECurrentWidgetStatusActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AECurrentWidgetStatusActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
