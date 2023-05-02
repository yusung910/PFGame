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
	//ApplyDamage�Լ��� �������� ���޹޴� �Լ�
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit", meta = (AllowPrivateAccess = true))
		bool isAir;


private:

	UPROPERTY()
		class UCharacterMovementComponent* CharMovementComp;


	UPROPERTY()
		float HitFlyingRate; //VLerp �Լ��� Alpha ���ڰ��� ���� ����

	UPROPERTY()
		bool isHitBound = false;

	UPROPERTY()
		bool isFalling = false; //�������� ������ ������, �ٴ����� �������� ������ ����

	UPROPERTY()
		bool isFlyMovement = false; //������ �����Ʈ ��尡 fly�������

	UPROPERTY()
		float fFallingRate; //VLerp �Լ��� Alpha ���ڰ��� ���� ����

	UPROPERTY()
		bool isFallingDown = false; //�߶�������

	UPROPERTY()
		bool isBeenDamaged = false; //�������� �Ծ�����

	UPROPERTY()
		float FallingToFloorRate; //������ ���� �ȴ� Rate

	UPROPERTY()
		float DamagedInAirLimitTimer = 2.0f; //�����ǰ� ���� ���� Ÿ�̸�


	UPROPERTY()
		float fKnockBackRate; //�˹��� ���ϴ� ��������

	UPROPERTY()
		float KnockBackRate; //VLerp �Լ��� Alpha ���ڰ��� ���� ����

	UPROPERTY()
		float HitYaw; //�ǰ� ����(Yaw)


	UPROPERTY()
		FHitResult hitResultInfo; //�ǰ� ������ ����Ǵ� ����ü

	UPROPERTY(VisibleAnywhere, Category = "EnemyMontage")
		class UAnimMontage* HitMontage;

	UPROPERTY(VisibleAnywhere, Category = "EnemyMontage")
		class UAnimMontage* AttackMontage;

	UPROPERTY(VisibleAnywhere, Category = "EnemyMontage")
		class UAnimMontage* LevelStartMontage;

	//

	//UPROPERTY(VisibleAnywhere, Category = "Collision")
	//	class UCapsuleComponent* Land;

	//ĳ������ �ٴ� ��ġ�� �����Ѵ�.
	float groundPosZ = 0.0f;

	float LevitationHeight;
	//Ÿ�̸� ���� �ð�
	float Timer = 0.0f;
	//���� ó�� ���� �� ��� ���ʹ� ��ü���� �� �ִ� �����̱� ������ �ʱ� ���� 3.0���� �Ҵ�
	float downTimer = 3.f;

	float currentFlightHeight;

	UPROPERTY()
		bool isBeingDragged = false; //�׷� ���ؼ� �������� ������

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

	//AI �۵� �� �����̴°��� ���߰� �ϱ� ���� �Լ�
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
