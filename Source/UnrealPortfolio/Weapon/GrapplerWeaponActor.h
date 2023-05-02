// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PlayerEnumActor.h"
#include "Components/TimeLineComponent.h"

#include "CoreMinimal.h"
#include "WeaponActor.h"
#include "GrapplerWeaponActor.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AGrapplerWeaponActor : public AWeaponActor
{
	GENERATED_BODY()
	
public:
	AGrapplerWeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* GrapMontage;

	UPROPERTY(VisibleAnywhere, Category = "Niagara")
		class UNiagaraComponent* Web;

	UPROPERTY(VisibleAnywhere, Category = "Niagara")
		class UNiagaraSystem* NWebSystem;

	UPROPERTY(VisibleAnywhere, Category = "Niagara")
		class USphereComponent* WebProjectile;

	//거미줄을 발사 했는지 여부
	bool isShootedWeb = false;

	//거미줄이 닿았는지
	bool isWebApproached = false;

	//Enemy가 플레이어 앞으로 도착했는지
	bool isEnemyArrival = false;

	float fWebDeleteTime = 0.f;

	float fWebVLerpRate = 0.f;
private:


	float attackRange = 200.f; //공격 길이
	float attackSpeed = 4.0f;  //공격 속도

	float damage = 10.f; //공격력

	bool havebeenGrapped = false; //그랩이 되어 있는지


protected:
	//거미줄이 충돌 했을 때 사용될 타임라인
	FTimeline WebHitTimeLine; 

	//거미줄 충돌 종료 되었 때 사용될 타임라인
	FTimeline StopEffectTimeLine;

	//
	FHitResult WebHitResult;

private:
	UFUNCTION()
		void WebHitTimeLineProgress(float Value);
	UFUNCTION()
		void WebStopEffectTimeLineProgress(float Value);

	void DoDragedEnemy();

	void MoveWeb(FVector s, FVector e);

public:
	UPROPERTY(EditAnywhere, Category = TimeLine)
		UCurveFloat* CurveWebHit;
	UPROPERTY(EditAnywhere, Category = TimeLine)
		UCurveFloat* CurveWebStopEffect;

private:
	virtual void Attack() override;
	virtual void SendDamage() override;
};
