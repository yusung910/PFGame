// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PlayerEnumActor.h"

#include "CoreMinimal.h"
#include "WeaponActor.h"
#include "SwordWeaponActor.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API ASwordWeaponActor : public AWeaponActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* AttackMontage1;
	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* UpperAttackMontage;
	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* UpperAttackFloorToJumpMontage;
	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* CurrentAttackMontage;
	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* AirAttackMontage;
	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* DashAttackMontage;
	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* DodgeMontage;
	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* ATFAttack3Montage;

	UPROPERTY()
		class UMainGameInstance* gi;

	float attackRange = 200.f; //공격 길이
	float attackRadius = 50.f; //공격 두께
	float attackSpeed = 1.2f;  //공격 속도


	TArray<class ACommonCharacter*> SendDamageToEnemyList; //데미지 전달 목록

	TArray<FHitResult> hitResults; //충돌 결과를 저장할 구조체 배열

	bool isExecutedTrace = false;

	float fKnockBackVal = 10.f;
public:
	ASwordWeaponActor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Attack() override;

	virtual void SubAttack() override;

	virtual void NextAttack() override;

	virtual void Dodge() override;

	virtual void NextATFAttack() override;

	virtual void EndATFLoop() override;

	virtual void SendDamage() override;

	virtual void KnockBackDamage() override;

	void SetupDamagedTarget(class APlayerCharacter* MyPlayer);
	void ApplyDamageToActor(class APlayerCharacter* MyPlayer);
	//void ApplyKnockBackDamageToActor(class APlayerCharacter* MyPlayer);
};
