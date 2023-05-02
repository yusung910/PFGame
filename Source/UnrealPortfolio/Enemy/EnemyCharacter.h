// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonCharacter.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class UNREALPORTFOLIO_API AEnemyCharacter : public ACommonCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	//ApplyDamage함수로 데미지를 전달받는 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit", meta = (AllowPrivateAccess = true))
		bool isAir;


private:

	UPROPERTY()
		class UCharacterMovementComponent* CharMovementComp;


	UPROPERTY()
		float HitFlyingRate; //VLerp 함수의 Alpha 인자값에 사용될 변수

	UPROPERTY()
		bool isHitBound = false;

	UPROPERTY()
		bool isFalling = false; //공중으로 오르는 중인지, 바닥으로 떨어지는 중인지 여부

	UPROPERTY()
		bool isFlyMovement = false; //액터의 무브먼트 모드가 fly모드인지

	UPROPERTY()
		float fFallingRate; //VLerp 함수의 Alpha 인자값에 사용될 변수

	UPROPERTY()
		bool isFallingDown = false; //추락중인지

	UPROPERTY()
		bool isBeenDamaged = false; //데미지를 입었는지

	UPROPERTY()
		float FallingToFloorRate; //땅으로 내리 꽂는 Rate

	UPROPERTY()
		float DamagedInAirLimitTimer = 2.0f; //공중피격 판정 제한 타이머


	UPROPERTY()
		float fKnockBackRate; //넉백을 당하는 상태인지

	UPROPERTY()
		float KnockBackRate; //VLerp 함수의 Alpha 인자값에 사용될 변수

	UPROPERTY()
		float HitYaw; //피격 방향(Yaw)


	UPROPERTY()
		FHitResult hitResultInfo; //피격 정보가 저장되는 구조체

	UPROPERTY(VisibleAnywhere, Category = "EnemyMontage")
		class UAnimMontage* HitMontage;

	UPROPERTY(VisibleAnywhere, Category = "EnemyMontage")
		class UAnimMontage* AttackMontage;

	UPROPERTY(VisibleAnywhere, Category = "EnemyMontage")
		class UAnimMontage* LevelStartMontage;

	//

	//UPROPERTY(VisibleAnywhere, Category = "Collision")
	//	class UCapsuleComponent* Land;

	//캐릭터의 바닥 위치를 저장한다.
	float groundPosZ = 0.0f;

	float LevitationHeight;
	//타이머 실행 시간
	float Timer = 0.0f;
	//게임 처음 시작 시 모든 에너미 객체들은 서 있는 상태이기 때문에 초기 값을 3.0으로 할당
	float downTimer = 3.f;

	float currentFlightHeight;

	UPROPERTY()
		bool isBeingDragged = false; //그랩 당해서 끌려가는 중인지

	bool isBeingGrabbed = false;

	FVector DraggedStart;
	FVector DraggedEnd;

	float DraggedRate = 0.0f;

	UPROPERTY()
		float DeathAfterGoneTimer = 0.0f;

	UPROPERTY(VisibleAnywhere)
		class UOrbsManagerComponent* OrbsComponent;

public:

	void DoDraggedCharacter(FVector s, FVector e);

	virtual void Attack() override;

	void SendDamage();

	//AI 작동 중 움직이는것을 멈추게 하기 위한 함수
	void ImmediatelyStopMove();

	
	

	FORCEINLINE bool GetIsHitFlying() { return isHitFlying; }

	FORCEINLINE float GetDownTimer() { return downTimer; }

	FORCEINLINE void SetIsHitBound(bool val) { isHitBound = val; }

	FORCEINLINE bool GetIsFalling() { return isFalling; }

	FORCEINLINE bool GetIsBeingGrabbed() { return isBeingGrabbed; }
	FORCEINLINE void SetIsBeingGrabbed(bool val) { isBeingGrabbed = val; }

	FORCEINLINE void SetCurrentFlightHeight(float val) { currentFlightHeight = val; }


	FORCEINLINE bool GetIsAir() { return isAir; }
	FORCEINLINE void SetIsAir(bool val) { isAir = val; }

	
};
