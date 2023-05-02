// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPlayerControllerInputFlag.h"

#include "MainPlayerController.generated.h"


/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMainPlayerController();

	//GameInstance
	UPROPERTY()
		class UMainGameInstance* gi;

	UPROPERTY()
		class AMainGameMode* MainGM;

protected:
	//플레이어와 빙의 시 호출 되는 함수(있어야 빙의 된다)
	virtual void OnPossess(APawn* InPawn) override;

	//입력받는함수
	virtual void SetupInputComponent() override;

	//플레이어 틱 함수
	virtual void PlayerTick(float DeltaTime) override;

private: //입력시 호출 될 함수
	UFUNCTION()
		void MoveForward(float value);

	UFUNCTION()
		void MoveRight(float value);

	UFUNCTION()
		void Zoom(float value);

	UFUNCTION()
		void Turn(float value);

	UFUNCTION()
		void Lookup(float value);

	UFUNCTION()
		void PlayerJump();

	UFUNCTION()
		void PlayerDodge();

	UFUNCTION()
		void PlayerAttackPressed();

	UFUNCTION()
		void PlayerAttackReleased();

	UFUNCTION()
		void PlayerSubAttack();

	UFUNCTION()
		void PlayerSetHandGun();

	UFUNCTION()
		void GetTraceTarget();

	UFUNCTION()
		void SetUnTarget();

	UFUNCTION()
		void ShowStore();

	UFUNCTION()
		void BlockStart();

	UFUNCTION()
		void PushEscButton();

	UFUNCTION()
		void UltimateSkill();

	UFUNCTION()
		void ShowMenu();

private:
	UPROPERTY()
		class APlayerCharacter* MyPlayer;

	UPROPERTY()
		int32 PressJumpCnt = 0;

	UPROPERTY()
		bool isCameraMove = false;

	UPROPERTY()
		float timer = 0.0f;

	UPROPERTY()
		float chargeTime = 100.f;

	UPROPERTY()
		FPlayerControllerInputFlag fInputFlag;

	UPROPERTY()
		bool isPressed = false;
public:

	FORCEINLINE FPlayerControllerInputFlag GetControllerInputFlag() { return fInputFlag; }
};