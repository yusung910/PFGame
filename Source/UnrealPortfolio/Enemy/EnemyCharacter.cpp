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

	//Ÿ�� widget ����
	TargetWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetWidget"));

	TSubclassOf<UTargetWidget> targetAsset;
	GetClassAsset(targetAsset, UTargetWidget, "WidgetBlueprint'/Game/Blueprints/Widgets/WB_Target.WB_Target_C'");

	TargetWidget->SetWidgetClass(targetAsset);
	TargetWidget->SetDrawSize(FVector2D(100.f, 100.f));

	TargetWidget->SetupAttachment(GetMesh());
	TargetWidget->SetRelativeLocation(FVector(0, 0, 100.f));
	TargetWidget->SetWidgetSpace(EWidgetSpace::Screen);

	TargetWidget->SetVisibility(false);

	//hit ��Ÿ�� ����
	GetObjectAsset(HitMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Enemy/HitReact_Montage.HitReact_Montage'");

	currentFlightHeight = GetFlightTimeLimit();

	//���� ��Ÿ�� ����
	GetObjectAsset(AttackMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Enemy/Enemy_Attack_Montage.Enemy_Attack_Montage'");
	
	//���� ��ŸƮ ��Ÿ��
	GetObjectAsset(LevelStartMontage, UAnimMontage, "AnimMontage'/Game/ParagonTerra/Characters/Heroes/Terra/Animations/LevelStart_Montage.LevelStart_Montage'");

	//AI ����
	//�ڵ����� ����� AIController
	AIControllerClass = AEnemyAIController::StaticClass();

	//Disabled: Feature is disabled (do not automatically possess AI). */
	//PlacedInWorld : Only possess by an AI Controller if Pawn is placed in the world.
	//Spawned : Only possess by an AI Controller if Pawn is spawned after the world has loaded.
	//PlacedInWorldOrSpawned: Pawn is automatically possessed by an AI Controller whenever it is created.
	//EAutoPossessAI : ���� �� ����ɰ�����, ������ ��ġ���� �� ����� ������.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;



	//Orbs������Ʈ ����
	OrbsComponent = CreateDefaultSubobject<UOrbsManagerComponent>(TEXT("OrbsComponent"));

	//
	GetCharacterMovement()->GravityScale = fGravityLimit; //�߷�
	GetCharacterMovement()->JumpZVelocity = fJumpVelocity; //ĳ���� ���� ����

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

	//���� ��ŸƮ ��Ÿ�� ���
	PlayAnimMontage(LevelStartMontage, 1.2f);

	//������ ���۽� ���ʹ� ��ü�� ���� ��Ų��
	//�׷��� AI�� ���� �۵�, ������ �𸣰���
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
		//���� ����
		isAir = GetMovementComponent()->IsFalling();

		//�ö��� ������� 
		isFlyMovement = (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying);

		//�������� ������ ������, �ٴ����� �������� ������ ����
		isFalling = (GetVelocity().Z > 0) ? false : true;

		//ĳ���� �ٿ� Ÿ�̸Ӹ� �����ϱ� ������
		//�������� ���ٰ� ������ �������� �ð��� �����ϱ� ����
		downTimer = downTimer + DeltaTime;

		//���߿��� �ǰ� ���ѽð��� �����Ѵ�.
		DamagedInAirLimitTimer = DamagedInAirLimitTimer + DeltaTime;

		isBeenDamaged = (DamagedInAirLimitTimer < 2.0f) ? true : false;

		//���߿� �߰ų� �׷������� ��� �ٿ� ����
		if (GetVelocity().Z != 0.f || isBeingGrabbed)
		{
			//ĳ���Ͱ� �������� �߱� �������� �� 0���� �ʱ�ȭ
			downTimer = .0f;
		}
		else
		{
			isBeingKnockBack = false;
		}

		//downTimer�� 3.0f���� Ŭ ��� �Ͼ �ִ� ����, 3.0f �����̸鼭
		//�����Ʈ ��尡 MOVE_Falling�ϰ��
		//isBeingKnockBack(�˹� ���ϴ� ������)�� false���
		isDown = (downTimer > 3.0f && GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Flying && !isBeingKnockBack) ? false : true;


		//���͸� �������.
		MovingActor(DraggedStart, DraggedEnd, isBeingGrabbed, DraggedRate, 0.08f);

		//�׷��� �Ǿ��� ���
		if (isBeingGrabbed)
		{
			//GetCharacterMovement()->StopActiveMovement();
			GetCharacterMovement()->StopMovementImmediately();
			//�׷��� �� ��ġ�� ������ ���
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

		//���͸� �������� ����
		FVector FirstAir = GetActorLocation(); //���� ������ ��ġ
		FirstAir.Z = LevitationHeight; //�������� ��� ���� ��
		MovingActor(GetActorLocation(), FirstAir, isHitFlying, HitFlyingRate, 0.01f);

		//���͸� �ٴڿ� �ȴ´�
		FVector FloorVec = GetActorLocation();
		FloorVec.Z = GetCharacterMovement()->CurrentFloor.HitResult.Location.Z;
		MovingActor(GetActorLocation(), FloorVec, isFallingToFloor, fFallingRate, 0.005f);
		//������ ���°� �ö��� ����̸鼭, ���� �ǰ��� ����Ǿ��� ���
		//���߿��� �ǰ� ������ ���� ��� ������ �߶���Ų��.
		if (isFlyMovement && !isHitFlying && !isBeenDamaged)
		{
			FVector dropDown = GetCharacterMovement()->CurrentFloor.HitResult.Location;
			MovingActor(GetActorLocation(), dropDown, isFallingDown, fFallingRate, 0.01f);
			//������ ��ġ(Z ��)�� �ٴ����� �������� ��� �����Ʈ ��带 ����
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
		//�������� �� ĳ����(�÷��̾�)�� ������ �������� ���� ������ ���;
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
			//���� �ǰݽ� �ö��� ���� ��������
			//�ش� ���¿��� �������� ���� ��� ���� ü�� ���ѽð��� 0���� �ʱ�ȭ�Ѵ�.
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


	//AI�� �������� �Ծ����� �����ϱ� ���� �Լ���.
	//Enemy ĳ���Ͱ� �������� �Ծ��� ���  AI��Ʈ�ѷ��� ���� �� �� �ִ� �Լ�
	UAISense_Damage::ReportDamageEvent(GetWorld(), this, Causer, DamageAmount, GetActorLocation(), Causer->GetActorLocation());

	//���ʹ� ��ü�� ���� �ִ� ������ �÷��̾�� ������
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
