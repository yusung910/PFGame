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
};	//������ ����

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


	//���⺰ �˹� ��(float)�� ���޹޾� �ش� �� ��ŭ ĳ���͸� �˹��Ų��
	virtual void MoveToKnockBack(FVector dir) {};

	//ĳ���� ���� �Լ�
	virtual void Attack() {};

	//ĳ���� �ٿ� �Լ�
	virtual void Down() {};

	//Vlerp�� �̿��ؼ� ���͸� Ư�� ��ġ�� �ű�� �Լ�
	virtual void MovingActor(FVector s, FVector e, bool& flag, float& rate, float increaseRate);

	UPROPERTY()
		bool isOrbDrop = false; //���� ���

	UPROPERTY()
		FName DropOrbsType;

	UPROPERTY()
		int32 OrbsAmount;

	UPROPERTY(VisibleAnywhere, Category = "Hit")
		class UNiagaraComponent* HitBlood1;

	//�ǰ� ��ƼŬ ���
	void PlayHittedEffect();

protected:
	//EnemyActor���� �÷��̾�� ������ �ֱ� ���� �Լ�.
	void SendDamageToPlayer(FVector TraceStart, FVector TraceEnd, EAttackType attackType);


	//ApplyDamage�Լ��� �������� ���޹޴� �Լ�
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

	//��� ĳ���Ͱ� ���� �޴� �߷� ��ġ
	UPROPERTY()
		float fGravityLimit = 4.0f;

	//���� ��ġ
	UPROPERTY()
		float fJumpVelocity = 2000.f;

	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* Land;

	UPROPERTY()
		class UWidgetComponent* TargetWidget;

	UPROPERTY()
		bool isDeath = false; //���

	UPROPERTY()
		bool isDown = false; //������ ���°� �����ִ� ��������

	UPROPERTY()
		bool DoPlayDownAnim = false; //���� �ٿ� �ִ� ��� ����


	UPROPERTY()
		bool isFallingToFloor = false; // ������ ��ι��� ġ����

	UPROPERTY()
		bool isHitFlying = false; //���� �ǰ� ��

	UPROPERTY()
		FVector HitLoc; // �ǰ� ����Ʈ�� ���� ��Ʈ ��ġ

	UPROPERTY()
		bool isBeingKnockBack = false; //�˹��� ���ϴ� ��������

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
