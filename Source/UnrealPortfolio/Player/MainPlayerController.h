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
	//�÷��̾�� ���� �� ȣ�� �Ǵ� �Լ�(�־�� ���� �ȴ�)
	virtual void OnPossess(APawn* InPawn) override;

	//�Է¹޴��Լ�
	virtual void SetupInputComponent() override;

	//�÷��̾� ƽ �Լ�
	virtual void PlayerTick(float DeltaTime) override;

private: //�Է½� ȣ�� �� �Լ�
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