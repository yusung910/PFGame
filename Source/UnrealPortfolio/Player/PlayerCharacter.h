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

	//블루프린트 AnyDamage의 C++버전
	//ApplyDamage를 통해 들어온 데미지를 전달받는다.
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void CheckJumpInput(float DeltaTime) override;
	virtual void Jump() override; //점프
	void DoDblJump();
	void Dodge(); //회피

	template<class T>
	void CallWeaponActorAndAttach(FName socketName, FName weaponName);

	void LookAtTarget();

	//체력에 따른 머티리얼 색상을 변환한다.
	void SetClothMaterialByHp(bool isDmg);

	//카메라진동
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
		TSubclassOf<class UUserWidget> PlayerHud; //메인 화면 위젯


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




private: //공격

	bool isAttack = false; //공격 중인지

	bool isMove = true; //이동 할 수 있는지

	bool isAir = false; //점프중인지

	bool isFalling = false; //바닥으로 추락하는 중인지(true: 추락, false: 상승)

	bool isAdditionJump = false; //추가 점프 가능한지

	bool isUpperAttack = false; //공중 어택기술 사용중인지

	bool isFloorToAirAttack = false; //바닥에서 공중으로 띄우는 기술이면서 같이 공중에 뜨는지

	bool isDashAttack = false; //대시 어택 중인지

	bool isDodge = false; //회피중인지

	bool isBlock = false; //방어중인지

	bool isATFAttack = false; //공중에서 땅으로 공격하는 기술 사용중인지

	bool isbeingGrapped = false;  //그랩 하는 중인지

	bool isUsingStore = false; //상점을 이용하는 중인지

	EPlayerWeapon currentWeaponType;

	float currentFlightHeight; //현재 점프 높이(LaunchCharacter 함수에 사용될 변수)

	float fDistScopeNPC = 200.0f;

	bool isGotHit = false; //피격당한 상태인지

	bool isKnockBackAttack = false; //넉백 공격인지

	bool isUltimateStatus = false;

	bool beDamagedKnockback = false; //넉백 데미지를 입었는지

	bool isBeingGetUp = false; //일어나는 중인지

	bool isCurrentAir = false;
private: //점프
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
	//float damage = 10.f; //데미지

	float lockOnDistance = 2000.f; //타겟 설정 유효 거리

	float LookAtPitch; // 타겟이 위챃고 있는 pitch

	float levitiationGravityScale = 1.0f;

	float Timer = 0.f;

	TArray<ACommonCharacter*> targetList; //targeting 객체 목록

	ACommonCharacter* lockEnemy;

	bool isNearestNPC = false;

	float HitYaw; //피격 방향(Yaw)

	float fCharMaxJump;

	float UltimateTimer;
	
	float MoveSpeed = 600.f;

	float DownDurationTime = 3.0f;
	float DownTimer;
public:
	void SetTarget(TArray<FHitResult> hitResults); //타겟 설정
	void GetTargetEnemy(); //타게팅 하는 Enemy 캐릭터들을 호출한다.
	void UnTargetingEnemy(); //타게팅 해제
	void DoGrap(); //공중 잡기(hook)

	void DoBlock(FName montageSecNm); //블락 상태

	bool GetIsAirPosition();

	//공중에서 Enemy 객체를 Grab해서 끌어 올릴 때 캐릭터를 한번 공중으로 점프 시킨다.
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