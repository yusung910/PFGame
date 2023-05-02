// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PlayerEnumActor.h"

#include "CoreMinimal.h"
#include "CommonCharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class UNREALPORTFOLIO_API APlayerCharacter : public ACommonCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//�������Ʈ AnyDamage�� C++����
	//ApplyDamage�� ���� ���� �������� ���޹޴´�.
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void CheckJumpInput(float DeltaTime) override;
	virtual void Jump() override; //����
	void DoDblJump();
	void Dodge(); //ȸ��

	template<class T>
	void CallWeaponActorAndAttach(FName socketName, FName weaponName);

	void LookAtTarget();

	//ü�¿� ���� ��Ƽ���� ������ ��ȯ�Ѵ�.
	void SetClothMaterialByHp(bool isDmg);

	//ī�޶�����
	void DoCameraShake();

	UPROPERTY()
		class UMainGameInstance* gi;

	UPROPERTY()
		class AMainGameMode* MainGM;
private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		FRotator springRot = FRotator(0.f, -180.f, 0.f);

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		float springArmLengthDefault = 200.f;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		FVector playerToTargetCenter;

	UPROPERTY(VisibleAnywhere, Category = "MOVING")
		float Direction;

	UPROPERTY(VisibleAnywhere, Category = "Physics")
		class UPhysicsHandleComponent* physicsHandle;

	UPROPERTY(VisibleAnywhere)
		TArray<class UMaterialInstanceDynamic*> Materials;

	UPROPERTY(VisibleAnywhere)
		TArray<class UMaterialInstanceDynamic*> UltimateMaterials;

	UPROPERTY(VisibleAnywhere)
		TArray<class UMaterialInstanceDynamic*> HpStatusMaterials;

	UPROPERTY(VisibleAnywhere, Category = "Orbs")
		class UOrbsManagerComponent* OrbsComponent;

	UPROPERTY()
		TSubclassOf<class UUserWidget> PlayerHud; //���� ȭ�� ����


public:
	virtual void Attack() override;

	virtual void Down() override;

	void GetUpCharacter();

	void Zoom(float value);
	void SubAttack();

	void SetSpringArmYaw(float val);
	void SetSpringArmPitch(float val);

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		TArray<AActor*> WeaponList;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		class ASwordWeaponActor* sword;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		class AHandGunWeaponActor* handgun;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		class AGrapplerWeaponActor* grap;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		class AWeaponActor* currentWeapon;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		class AWeaponActor* subWeapon;
	UPROPERTY(VisibleAnywhere, Category = "Enemy")
		class AEnemyCharacter* enemy;

	UPROPERTY(VisibleAnywhere, Category = "NPC")
		TArray<AActor*> NPCList;




private: //����

	bool isAttack = false; //���� ������

	bool isMove = true; //�̵� �� �� �ִ���

	bool isAir = false; //����������

	bool isFalling = false; //�ٴ����� �߶��ϴ� ������(true: �߶�, false: ���)

	bool isAdditionJump = false; //�߰� ���� ��������

	bool isUpperAttack = false; //���� ���ñ�� ���������

	bool isFloorToAirAttack = false; //�ٴڿ��� �������� ���� ����̸鼭 ���� ���߿� �ߴ���

	bool isDashAttack = false; //��� ���� ������

	bool isDodge = false; //ȸ��������

	bool isBlock = false; //���������

	bool isATFAttack = false; //���߿��� ������ �����ϴ� ��� ���������

	bool isbeingGrapped = false;  //�׷� �ϴ� ������

	bool isUsingStore = false; //������ �̿��ϴ� ������

	EPlayerWeapon currentWeaponType;

	float currentFlightHeight; //���� ���� ����(LaunchCharacter �Լ��� ���� ����)

	float fDistScopeNPC = 200.0f;

	bool isGotHit = false; //�ǰݴ��� ��������

	bool isKnockBackAttack = false; //�˹� ��������

	bool isUltimateStatus = false;

	bool beDamagedKnockback = false; //�˹� �������� �Ծ�����

	bool isBeingGetUp = false; //�Ͼ�� ������

	bool isCurrentAir = false;
private: //����
	//montage
	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* AdditionJumpMontage;

	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* GunHitMontage;

	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* SwordHitMontage;

	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* SwordBlockHitMontage;

	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* DownMontage;

	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* BlockMontage;
private:
	//float damage = 10.f; //������

	float lockOnDistance = 2000.f; //Ÿ�� ���� ��ȿ �Ÿ�

	float LookAtPitch; // Ÿ���� ���S�� �ִ� pitch

	float levitiationGravityScale = 1.0f;

	float Timer = 0.f;

	TArray<ACommonCharacter*> targetList; //targeting ��ü ���

	ACommonCharacter* lockEnemy;

	bool isNearestNPC = false;

	float HitYaw; //�ǰ� ����(Yaw)

	float fCharMaxJump;

	float UltimateTimer;
	
	float MoveSpeed = 600.f;

	float DownDurationTime = 3.0f;
	float DownTimer;
public:
	void SetTarget(TArray<FHitResult> hitResults); //Ÿ�� ����
	void GetTargetEnemy(); //Ÿ���� �ϴ� Enemy ĳ���͵��� ȣ���Ѵ�.
	void UnTargetingEnemy(); //Ÿ���� ����
	void DoGrap(); //���� ���(hook)

	void DoBlock(FName montageSecNm); //��� ����

	bool GetIsAirPosition();

	//���߿��� Enemy ��ü�� Grab�ؼ� ���� �ø� �� ĳ���͸� �ѹ� �������� ���� ��Ų��.
	void DoAirLaunchCharacter();

	UFUNCTION()
		void DeadAfter();

public:

	FORCEINLINE bool GetIsAttack() { return isAttack; }
	FORCEINLINE void SetIsAttack(bool value) { isAttack = value; }

	FORCEINLINE bool GetIsMove() { return isMove; }
	FORCEINLINE void SetIsMove(bool value) { isMove = value; }

	FORCEINLINE bool GetIsAdditionJump() { return isAdditionJump; }
	FORCEINLINE void SetIsAdditionJump(bool value) { isAdditionJump = value; }

	FORCEINLINE bool GetIsAir() { return isAir; }
	FORCEINLINE void SetIsAir(bool value) { isAir = value; }

	FORCEINLINE float GetLockOnDistance() { return lockOnDistance; }
	FORCEINLINE void SetLockOnDistance(float val) { lockOnDistance = val; }

	FORCEINLINE bool GetIsLockOnEnemy() { return (lockEnemy != nullptr) ? true : false; }

	FORCEINLINE ACommonCharacter* GetLockEnemy() { return lockEnemy; }

	FORCEINLINE EPlayerWeapon GetCurrentWeaponType() { return currentWeaponType; }

	void SetCurrentWeaponType(EPlayerWeapon val);

	FORCEINLINE FRotator GetSpringRot() { return springRot; }

	//playerToTargetCenter
	FORCEINLINE FVector GetPlayerToTargetCenter() { return playerToTargetCenter; }

	FORCEINLINE AWeaponActor* GetCurrentWeaponObj() { return currentWeapon; }

	FORCEINLINE bool GetIsUpperAttack() { return isUpperAttack; }
	FORCEINLINE void SetIsUpperAttack(bool val) { isUpperAttack = val; }

	//isDashAttack
	FORCEINLINE bool GetIsDashAttack() { return isDashAttack; }
	FORCEINLINE void SetIsDashAttack(bool val) { isDashAttack = val; }

	FORCEINLINE float GetDirection() { return Direction; }

	FORCEINLINE bool GetIsDodge() { return isDodge; }
	FORCEINLINE void SetIsDodge(bool val) { isDodge = val; }

	FORCEINLINE bool GetIsATFAttack() { return isATFAttack; }
	FORCEINLINE void SetIsATFAttack(bool val) { isATFAttack = val; }

	FORCEINLINE bool GetIsFalling() { return isFalling; }

	FORCEINLINE float GetLookAtPitch() { return LookAtPitch; }

	FORCEINLINE bool GetIsBlock() { return isBlock; }
	FORCEINLINE void SetIsBlock(bool val) { isBlock = val; }

	FORCEINLINE bool GetIsbeingGrapped() { return isbeingGrapped; }
	FORCEINLINE void SetIsbeingGrapped(bool val) { isbeingGrapped = val; }

	FORCEINLINE void SetCurrentFlightHeight(float val) { currentFlightHeight = val; }

	FORCEINLINE bool GetIsFloorToAirAttack() { return isFloorToAirAttack; }
	FORCEINLINE void SetIsFloorToAirAttack(bool val) { isFloorToAirAttack = val; }

	FORCEINLINE bool GetIsNearestNPC() { return isNearestNPC; }

	FORCEINLINE bool GetIsUsingStore() { return isUsingStore; }
	FORCEINLINE void SetIsUsingStore(bool val) { isUsingStore = val; }


	FORCEINLINE float GetCharMaxJump() { return fCharMaxJump; }

	FORCEINLINE bool GetIsKnockBackAttack() { return isKnockBackAttack; }
	FORCEINLINE void SetIsKnockBackAttack(bool val) { isKnockBackAttack = val; }

	FORCEINLINE bool GetIsUltimateStatus() { return isUltimateStatus; }
	FORCEINLINE void SetIsUltimateStatus(bool val) { isUltimateStatus = val; }

	FORCEINLINE UOrbsManagerComponent* GetOrbsComponent() { return OrbsComponent; }

	FORCEINLINE float GetDownDurationTime() { return DownDurationTime; }
	FORCEINLINE void SetDownDurationTime(float f) { DownDurationTime = f; }

	FORCEINLINE void SetBeDamagedKnockback(bool b) { beDamagedKnockback = b; }

	FORCEINLINE bool GetIsBeingGetUp() { return isBeingGetUp; }
	FORCEINLINE void SetIsBeingGetUp(bool b) { isBeingGetUp = b; }
	
	FORCEINLINE float GetMoveSpeed() { return MoveSpeed; }
};