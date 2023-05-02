// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CommonCharacter.generated.h"


UENUM(BlueprintType)
enum class EAttackType : uint8
{
	SINGLE		UMETA(DisplayName = "SINGLE"),
	MULTI		UMETA(DisplayName = "MULTI"),
};	//열거형 정의

UCLASS()
class UNREALPORTFOLIO_API ACommonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACommonCharacter();
	float GetCharacterCalculateDirection(const FVector Velocity, const FRotator BaseRotation);

	void SetVisiblityTargetWidget(bool isShow);

	void SetGravity(float val);

	FName GetHitDirection(float HitYaw);

	void CharacterDeath(UAnimMontage* deathMontage);

	void KnockBackCharacter(FVector dir);

	void OrbsSend(AActor* receiveActor);
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//무기별 넉백 값(float)를 전달받아 해당 값 만큼 캐릭터를 넉백시킨다
	virtual void MoveToKnockBack(FVector dir) {};

	//캐릭터 어택 함수
	virtual void Attack() {};

	//캐릭터 다운 함수
	virtual void Down() {};

	//Vlerp를 이용해서 액터를 특정 위치에 옮기는 함수
	virtual void MovingActor(FVector s, FVector e, bool& flag, float& rate, float increaseRate);

	UPROPERTY()
		bool isOrbDrop = false; //오브 드랍

	UPROPERTY()
		FName DropOrbsType;

	UPROPERTY()
		int32 OrbsAmount;

	UPROPERTY(VisibleAnywhere, Category = "Hit")
		class UNiagaraComponent* HitBlood1;

	//피격 파티클 재생
	void PlayHittedEffect();

protected:
	//EnemyActor들이 플레이어에게 데미지 주기 위한 함수.
	void SendDamageToPlayer(FVector TraceStart, FVector TraceEnd, EAttackType attackType);


	//ApplyDamage함수로 데미지를 전달받는 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY()
		float attackRange;

	UPROPERTY()
		float attackRadius;

	UPROPERTY()
		float radialAttackRadius;

	UPROPERTY()
		float DefaultRadialAttackRadius;

	//모든 캐릭터가 적용 받는 중력 수치
	UPROPERTY()
		float fGravityLimit = 4.0f;

	//점프 수치
	UPROPERTY()
		float fJumpVelocity = 2000.f;

	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* Land;

	UPROPERTY()
		class UWidgetComponent* TargetWidget;

	UPROPERTY()
		bool isDeath = false; //사망

	UPROPERTY()
		bool isDown = false; //액터의 상태가 누워있는 상태인지

	UPROPERTY()
		bool DoPlayDownAnim = false; //액터 다운 애님 재생 여부


	UPROPERTY()
		bool isFallingToFloor = false; // 땅으로 곤두박질 치는지

	UPROPERTY()
		bool isHitFlying = false; //공중 피격 시

	UPROPERTY()
		FVector HitLoc; // 피격 이펙트를 위한 히트 위치

	UPROPERTY()
		bool isBeingKnockBack = false; //넉백을 당하는 상태인지

	UPROPERTY()
		float damage;
	UPROPERTY()
		float DefaultDamage;
	UPROPERTY()
		float currentHP;
	UPROPERTY()
		float maxHP;
	UPROPERTY()
		float DefaultMoveSpeed = 600.f;
	UPROPERTY()
		float DefaultJumpVelocity = 400.f;

	UPROPERTY()
		float DestroyTimeAccumulate = 0.0f;

	UPROPERTY()
		float DestroyLimitTime = 2.0f;
	bool isOrbsDrop = false;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FORCEINLINE float GetFlightTimeLimit() { return flightTimeLimit; }

	FORCEINLINE float GetGravityLimit() { return fGravityLimit; }

	FORCEINLINE bool GetIsDeath() { return isDeath; }

	FORCEINLINE void SetIsHitFlying(bool val) { isHitFlying = val; }

	FORCEINLINE void FallingToFloor() { isFallingToFloor = true; }
	FORCEINLINE bool GetFallingToFloor() { return isFallingToFloor; }

	FORCEINLINE void SetHitLoc(FVector vec) { HitLoc = vec; }

	FORCEINLINE bool GetIsBeingKnockBack() { return isBeingKnockBack; }
	FORCEINLINE void SetIsBeingKnockBack(bool val) { isBeingKnockBack = val; }

	FORCEINLINE bool GetIsDown() { return isDown; }
	FORCEINLINE void SetIsDown(bool val) { isDown = val; }

	FORCEINLINE float GetCurrentHP() { return currentHP; }
private:
	float flightTimeLimit = 1000.f;

};
