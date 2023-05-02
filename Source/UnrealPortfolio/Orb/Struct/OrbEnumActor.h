// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OrbEnumActor.generated.h"

UENUM(BlueprintType)
enum class EOrbScale: uint8
{
	ONE		UMETA(DisplayName = "1x"),
	THREE	UMETA(DisplayName = "3x"),
	FIVE	UMETA(DisplayName = "5x"),
	SEVEN	UMETA(DisplayName = "7x"),
	TEN		UMETA(DisplayName = "10x"),
	FIFTEEN UMETA(DisplayName = "15x"),

};	//열거형 정의


UCLASS()
class UNREALPORTFOLIO_API AOrbEnumActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOrbEnumActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
