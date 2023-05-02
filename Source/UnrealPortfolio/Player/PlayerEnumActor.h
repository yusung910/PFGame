// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerEnumActor.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	IDLE	UMETA(DisplayName = "idle"),
	MOVE	UMETA(DisplayName = "move"),
	RUN		UMETA(DisplayName = "run"),
	ATTACK	UMETA(DisplayName = "attack"),
};	//열거형 정의

UENUM(BlueprintType)
enum class EPlayerWeapon : uint8
{
	UNARMED	UMETA(DisplayName = "unarmed"),
	SWORD	UMETA(DisplayName = "sword"),
	HANDGUN	UMETA(DisplayName = "handgun"),
	GRAP	UMETA(DisplayName = "grap")
};	//열거형 정의


UCLASS()
class UNREALPORTFOLIO_API APlayerEnumActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerEnumActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
