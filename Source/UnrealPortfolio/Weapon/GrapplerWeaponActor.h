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

	//�Ź����� �߻� �ߴ��� ����
	bool isShootedWeb = false;

	//�Ź����� ��Ҵ���
	bool isWebApproached = false;

	//Enemy�� �÷��̾� ������ �����ߴ���
	bool isEnemyArrival = false;

	float fWebDeleteTime = 0.f;

	float fWebVLerpRate = 0.f;
private:


	float attackRange = 200.f; //���� ����
	float attackSpeed = 4.0f;  //���� �ӵ�

	float damage = 10.f; //���ݷ�

	bool havebeenGrapped = false; //�׷��� �Ǿ� �ִ���


protected:
	//�Ź����� �浹 ���� �� ���� Ÿ�Ӷ���
	FTimeline WebHitTimeLine; 

	//�Ź��� �浹 ���� �Ǿ� �� ���� Ÿ�Ӷ���
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
