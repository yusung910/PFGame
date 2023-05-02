// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonCharacter.h"
#include "BossCharacter.generated.h"

UCLASS()
class UNREALPORTFOLIO_API ABossCharacter : public ACommonCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//ApplyDamage함수로 데미지를 전달받는 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Attack();

	void NextAttack();

	void ShotSpell();

	void SendDamage();

	void RadialDamage();

	void BossPowerUp(int32 n);

	void SetLandingLocation(AActor* Target);

	void LeapAttackStart();

	void LeapAttackLoopEnd();

	void SetPhaseMaterial();

	FORCEINLINE int32 GetBossPhase() { return Phase; }
	FORCEINLINE void InitBossAttackNum() { PlayAttackNum = 1; }
	FORCEINLINE float GetAttackRange() { return attackRange; }
	FORCEINLINE float GetStrafeRange() { return StrafeRange; }

	FORCEINLINE bool GetIsBeingLeapAttack() { return isBeingLeapAttack; }
	FORCEINLINE void SetIsBeingLeapAttack(bool val) { isBeingLeapAttack = val; }

	FORCEINLINE bool GetIsBeingAttack() { return isBeingAttack; }
	FORCEINLINE void SetIsBeingAttack(bool val) { isBeingAttack = val; }

	FORCEINLINE bool GetIsHadouKenAttack() { return isHadouKenAttack; }
	FORCEINLINE void SetIsHadouKenAttack(bool val) { isHadouKenAttack = val; }

	FORCEINLINE float GetLeapAttackCastingTime() { return LeapAttackCastingTime; }

	FORCEINLINE bool GetIsCoolTime() { return isCoolTime; }
	FORCEINLINE void SetIsCoolTime(bool b) { isCoolTime = b; }


	FORCEINLINE float GetHadokenDamage() { return HadokenDamage; }
private:
	UPROPERTY()
		class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* LevelStartMontage;

	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* BossAttackMontage;

	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* LeapMontage;

	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* HitMontage;

	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* DeathMontage;

	UPROPERTY(VisibleAnywhere)
		class UOrbsManagerComponent* OrbsComponent;

	UPROPERTY()
		class UMainGameInstance* gi;

	UPROPERTY()
		class AMainGameMode* MainGM;

	UPROPERTY(VisibleAnywhere, Category = "ObjectPooling")
		TArray<class ABossProjectileActor*> Spells; //오브젝트 풀을 관리 배열

	FVector LandingLoc;

	UPROPERTY(VisibleAnywhere)
		class UMaterialInstanceDynamic* PhaseMaterials;


	UPROPERTY()
		class ALandingMarkerActor* LandingLocActor;
private:
	float GroundSmashRadius = 200.f; //리프어택 영향 반지름

	float DefaultAttackRadius = 200.f;

	float StrafeRange = 1000.f; //회피 범위

	float HadokenDamage = 10.f;

	int32 Phase = 1;

	int32 PlayAttackNum = 1;

	bool isBeingLeapAttack = false;

	bool isBeingAttack = false;

	bool isHadouKenAttack = false;

	bool isCoolTime = false;

	float MontagePlaySpeed = 1.2f;

	float LeapAttackCastingTime;

	float HitYaw;

	float ClearTimer;

	bool isClear = false;

};
