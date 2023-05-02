// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ECurrentWidgetStatusActor.h"

#include "StartGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AStartGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AStartGameMode();

	virtual void BeginPlay() override;

	void RemoveMainWidget();

	void DoLoadData();

private:
	UPROPERTY()
		class UMainGameInstance* gi;

	UPROPERTY()
		class UUserWidget* CurrentWidget;

	UPROPERTY()
		TSubclassOf<class UUserWidget> IntroWidgetClass; //메인 화면 위젯

	UPROPERTY()
		class UDataSaveGame* LoadData;
};
