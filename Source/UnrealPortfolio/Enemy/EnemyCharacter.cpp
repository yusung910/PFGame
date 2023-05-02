// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"

#include "PlayerCharacter.h"
#include "WeaponActor.h"
#include "TargetWidget.h"
#include "EnemyAIController.h"

#include "OrbsManagerComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Perception/AISense_Damage.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "EnemyCharacter.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USkeletalMesh* meshAsset;
	GetObjectAsset(meshAsset, USkeletalMesh, "SkeletalMesh'/Game/ParagonTerra/Characters/Heroes/Terra/Skins/GryphonKnight/Meshes/Terra_GryphonKnight.Terra_GryphonKnight'");
	GetMesh()->SetSkeletalMesh(meshAsset);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));

	TSubclassOf<UAnimInstance> animAsset;
	GetClassAsset(animAsset, UAnimInstance, "AnimBlueprint'/Game/Blueprints/Anim/Enemy/ABP_Enemy.ABP_Enemy_C'");

	GetMesh()->AnimClass = animAsset;
	GetMesh()->CanCharacterStepUpOn = ECB_No;
	GetMesh()->SetCollisionProfileName(FName("PhysicsActor"));
	GetMesh()->SetWalkableSlopeOverride(FWalkableSlopeOverride(EWalkableSlopeBehavior::WalkableSlope_Unwalkable, 90.f));

	GetCapsuleComponent()->SetCollisionProfileName(FName("PhysicsActor"));
	GetCapsuleComponent()->SetWalkableSlopeOverride(FWalkableSlopeOverride(EWalkableSlopeBehavior::WalkableSlope_Unwalkable, 90.f));

	Tags.Add(FName(TEXT("Enemy")));

	//타겟 widget 생성
	TargetWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetWidget"));

	TSubclassOf<UTargetWidget> targetAsset;
	GetClassAsset(targetAsset, UTargetWidget, "WidgetBlueprint'/Game/Blueprints/Widgets/WB_Target.WB_Target_C'");

	TargetWidget->SetWidgetClass(targetAsset);
	TargetWidget->SetDrawSize(FVector2D(100.f, 100.f));

	TargetWidget->SetupAttachment(GetMesh());
	TargetWidget->SetRelativeLocation(FVector(0, 0, 100.f));
	TargetWidget->SetWidgetSpace(EWidgetSpace::Screen);

	TargetWidget->SetVisibility(false);

	//hit 몽타주 생성
	GetObjectAsset(HitMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Enemy/HitReact_Montage.HitReact_Montage'");

	currentFlightHeight = GetFlightTimeLimit();

	//공격 몽타주 생성
	GetObjectAsset(AttackMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Enemy/Enemy_Attack_Montage.Enemy_Attack_Montage'");
	
	//레벨 스타트 몽타주
	GetObjectAsset(LevelStartMontage, UAnimMontage, "AnimMontage'/Game/ParagonTerra/Characters/Heroes/Terra/Animations/LevelStart_Montage.LevelStart_Montage'");

	//AI 세팅
	//자동으로 연결될 AIController
	AIControllerClass = AEnemyAIController::StaticClass();

	//Disabled: Feature is disabled (do not automatically possess AI). */
	//PlacedInWorld : Only possess by an AI Controller if Pawn is placed in the world.
	//Spawned : Only possess by an AI Controller if Pawn is spawned after the world has loaded.
	//PlacedInWorldOrSpawned: Pawn is automatically possessed by an AI Controller whenever it is created.
	//EAutoPossessAI : 생성 시 연결될것인지, 레벨에 배치됐을 때 연결될 것인지.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;



	//Orbs컴포넌트 생성
	OrbsComponent = CreateDefaultSubobject<UOrbsManagerComponent>(TEXT("OrbsComponent"));

	//
	GetCharacterMovement()->GravityScale = fGravityLimit; //중력
	GetCharacterMovement()->JumpZVelocity = fJumpVelocity; //캐릭터 점프 높이

	damage = 5.0f;
	currentHP = 100.f;
	maxHP = 100.f;

	attackRange = 200.f;
	attackRadius = 20.f;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//레벨 스타트 몽타주 재생
	PlayAnimMontage(LevelStartMontage, 1.2f);

	//게임이 시작시 에너미 객체를 점프 시킨다
	//그래야 AI가 정상 작동, 이유는 모르겠음
	FVector jumpTmp = GetActorLocation();
	jumpTmp.Z += 50.f;
	LaunchCharacter(jumpTmp, false , false);
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (currentHP >= 0.0f)
	{
		//공중 판정
		isAir = GetMovementComponent()->IsFalling();

		//플라잉 모드인지 
		isFlyMovement = (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying);

		//공중으로 오르는 중인지, 바닥으로 떨어지는 중인지 여부
		isFalling = (GetVelocity().Z > 0) ? false : true;

		//캐릭터 다운 타이머를 측정하기 위함임
		//공중으로 떴다가 땅으로 떨어지는 시간을 측정하기 위함
		downTimer = downTimer + DeltaTime;

		//공중에서 피격 제한시간을 측정한다.
		DamagedInAirLimitTimer = DamagedInAirLimitTimer + DeltaTime;

		isBeenDamaged = (DamagedInAirLimitTimer < 2.0f) ? true : false;

		//공중에 뜨거나 그랩당했을 경우 다운 판정
		if (GetVelocity().Z != 0.f || isBeingGrabbed)
		{
			//캐릭터가 공중으로 뜨기 시작했을 때 0으로 초기화
			downTimer = .0f;
		}
		else
		{
			isBeingKnockBack = false;
		}

		//downTimer가 3.0f보다 클 경우 일어나 있는 상태, 3.0f 이하이면서
		//무브먼트 모드가 MOVE_Falling일경우
		//isBeingKnockBack(넉백 당하는 중인지)가 false경우
		isDown = (downTimer > 3.0f && GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Flying && !isBeingKnockBack) ? false : true;


		//액터를 끌어당긴다.
		MovingActor(DraggedStart, DraggedEnd, isBeingGrabbed, DraggedRate, 0.08f);

		//그랩이 되었을 경우
		if (isBeingGrabbed)
		{
			//GetCharacterMovement()->StopActiveMovement();
			GetCharacterMovement()->StopMovementImmediately();
			//그랩이 된 위치가 공중일 경우
			if (GetActorLocation().Z > 200.f)
			{
				DamagedInAirLimitTimer = 0.f;
				GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
			}
			else
			{
				DamagedInAirLimitTimer = 2.f;
				GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Falling;
			}
		}

		//액터를 공중으로 띄운다
		FVector FirstAir = GetActorLocation(); //현재 액터의 위치
		FirstAir.Z = LevitationHeight; //공중으로 띄울 높이 값
		MovingActor(GetActorLocation(), FirstAir, isHitFlying, HitFlyingRate, 0.01f);

		//액터를 바닥에 꽂는다
		FVector FloorVec = GetActorLocation();
		FloorVec.Z = GetCharacterMovement()->CurrentFloor.HitResult.Location.Z;
		MovingActor(GetActorLocation(), FloorVec, isFallingToFloor, fFallingRate, 0.005f);
		//액터의 상태가 플라이 모드이면서, 공중 피격이 종료되었을 경우
		//공중에서 피격 판정이 없을 경우 땅으로 추락시킨다.
		if (isFlyMovement && !isHitFlying && !isBeenDamaged)
		{
			FVector dropDown = GetCharacterMovement()->CurrentFloor.HitResult.Location;
			MovingActor(GetActorLocation(), dropDown, isFallingDown, fFallingRate, 0.01f);
			//액터의 위치(Z 값)가 바닥으로 떨어졌을 경우 무브먼트 모드를 변경
			if (!isFallingDown)
			{
				GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Falling;
			}
		}

		if (isFallingToFloor)
		{
			GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Falling;
		}
	}
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	APlayerCharacter* Causer = Cast<APlayerCharacter>(DamageCauser);

	HitYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Causer->GetActorLocation()).Yaw;

	PlayAnimMontage(HitMontage, 1.2f, GetHitDirection(HitYaw));

	if (Causer->GetCurrentWeaponObj()->GetIsLevitationAttack())
	{
		//데미지를 준 캐릭터(플레이어)의 공격이 공중으로 띄우는 공격일 경우;
		//LOG_SCREEN("HIT LEVITATION");
		isHitFlying = true;
		GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
		DamagedInAirLimitTimer = 1.f;
		LevitationHeight = Causer->GetCharMaxJump();
	}

	if (isFlyMovement)
	{
		if (!isFallingToFloor)
		{
			//공중 피격시 플라잉 모드로 정해진다
			//해당 상태에서 데미지를 입을 경우 공중 체공 제한시간을 0으로 초기화한다.
			switch (Causer->GetCurrentWeaponType())
			{
				case EPlayerWeapon::HANDGUN:
					DamagedInAirLimitTimer -= 0.25f;
					break;
				case EPlayerWeapon::SWORD:
					DamagedInAirLimitTimer = 0.0f;
					break;
			}
		}
		else
		{
			DamagedInAirLimitTimer = 2.0f;
		}
	}


	//AI가 데미지를 입었는지 감지하기 위한 함수다.
	//Enemy 캐릭터가 데미지를 입었을 경우  AI컨트롤러가 감지 할 수 있는 함수
	UAISense_Damage::ReportDamageEvent(GetWorld(), this, Causer, DamageAmount, GetActorLocation(), Causer->GetActorLocation());

	//에너미 객체의 보고 있는 방향을 플레이어로 돌린다
	FRotator look = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Causer->GetActorLocation());

	SetActorRotation(FRotator(0, look.Yaw, 0));

	if (Causer->GetIsKnockBackAttack())
	{
		StopAnimMontage();
	}

	currentHP -= DamageAmount;

	UTargetWidget* TargetHpWidget = Cast<UTargetWidget>(TargetWidget->GetUserWidgetObject());
	if (TargetHpWidget == nullptr) return DamageAmount;
	TargetHpWidget->SetHPBarRatio(currentHP / maxHP);

	if (currentHP <= 0.0f)
	{
		StopAnimMontage();
		GetCharacterMovement()->StopMovementImmediately();
		KnockBackCharacter(Causer->GetActorForwardVector());
		TargetHpWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (!isOrbsDrop && currentHP <= 0.0f)
	{
		DropOrbsType = "red";
		OrbsAmount = 100;
		OrbsSend(Causer);
	}

	return DamageAmount;
}



void AEnemyCharacter::DoDraggedCharacter(FVector s, FVector e)
{
	isBeingGrabbed = true;
	DraggedStart = s;
	DraggedEnd = e;
}

void AEnemyCharacter::Attack()
{
	PlayAnimMontage(AttackMontage, 1.2f);
}


void AEnemyCharacter::SendDamage()
{
	SendDamageToPlayer(GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * attackRange), EAttackType::SINGLE);
}

void AEnemyCharacter::ImmediatelyStopMove()
{
	FVector Loc2D = ExceptZFVector(GetActorLocation());
	SetActorLocation(Loc2D);
}
