// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"

#include "TargetWidget.h"
#include "BossAIController.h"

#include "MainGameInstance.h"
#include "MainGameMode.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "PlayerCharacter.h"
#include "LandingMarkerActor.h"

#include "OrbsManagerComponent.h"
#include "CharacterCommonWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/WidgetComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "BossProjectileActor.h"

#include "BossCharacter.h"
// Sets default values
ABossCharacter::ABossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//staticMesh
	//StaticMeshComponent 생성
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));

	RootComponent->SetupAttachment(StaticMeshComp);

	USkeletalMesh* meshAsset;
	GetObjectAsset(meshAsset, USkeletalMesh, "SkeletalMesh'/Game/ParagonRampage/Characters/Heroes/Rampage/Skins/Tier2/Elemental/Meshes/Rampage_Elemental.Rampage_Elemental'");
	GetMesh()->SetSkeletalMesh(meshAsset);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));
	GetMesh()->SetRelativeScale3D(FVector(2.5f));

	TSubclassOf<UAnimInstance> animAsset;
	GetClassAsset(animAsset, UAnimInstance, "AnimBlueprint'/Game/Blueprints/Anim/Boss/ABP_Boss.ABP_Boss_C'");


	GetMesh()->AnimClass = animAsset;
	GetMesh()->CanCharacterStepUpOn = ECB_No;
	GetMesh()->SetCollisionProfileName(FName("PhysicsActor"));
	GetMesh()->SetWalkableSlopeOverride(FWalkableSlopeOverride(EWalkableSlopeBehavior::WalkableSlope_Unwalkable, 90.f));

	GetCapsuleComponent()->SetCollisionProfileName(FName("PhysicsActor"));
	GetCapsuleComponent()->SetWalkableSlopeOverride(FWalkableSlopeOverride(EWalkableSlopeBehavior::WalkableSlope_Unwalkable, 90.f));

	Tags.Add(FName(TEXT("Enemy")));
	Tags.Add(FName(TEXT("Boss")));

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

	GetObjectAsset(LevelStartMontage, UAnimMontage, "AnimMontage'/Game/ParagonRampage/Characters/Heroes/Rampage/Animations/LevelStart_Montage.LevelStart_Montage'");

	GetObjectAsset(BossAttackMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Boss/Boss_Attack_montage.Boss_Attack_Montage'");

	//
	GetObjectAsset(LeapMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Boss/Ability_GroundSmash_Montage.Ability_GroundSmash_Montage'");

	//
	GetObjectAsset(HitMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Boss/HitReact_Montage.HitReact_Montage'");

	//
	GetObjectAsset(DeathMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Boss/BossDieMontage.BossDieMontage'");
	

	//AI 세팅
	//자동으로 연결될 AIController
	AIControllerClass = ABossAIController::StaticClass();

	//Disabled: Feature is disabled (do not automatically possess AI). */
	//PlacedInWorld : Only possess by an AI Controller if Pawn is placed in the world.
	//Spawned : Only possess by an AI Controller if Pawn is spawned after the world has loaded.
	//PlacedInWorldOrSpawned: Pawn is automatically possessed by an AI Controller whenever it is created.
	//EAutoPossessAI : 생성 시 연결될것인지, 레벨에 배치됐을 때 연결될 것인지.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//Orbs컴포넌트 생성
	OrbsComponent = CreateDefaultSubobject<UOrbsManagerComponent>(TEXT("OrbsComponent"));

	damage = 5.0f;
	DefaultDamage = 5.0f;

	attackRange = 400.f;
	attackRadius = 150.f;
	DefaultRadialAttackRadius = 500.f;
	currentHP = 1000.0f;
	maxHP = 1000.0f;
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayAnimMontage(LevelStartMontage, MontagePlaySpeed);
	gi = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MainGM = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	FTransform transform;

	//현재 액터의 위치 + 액터의 앞방향 * 100.f
	transform.SetLocation(GetActorLocation() + GetActorForwardVector() * 100.f);

	//ShotActor 생성
	ABossProjectileActor* obj = GetWorld()->SpawnActor<ABossProjectileActor>(ABossProjectileActor::StaticClass(), transform);

	obj->SetActive(false);
	Spells.Add(obj);

	PhaseMaterials = GetMesh()->CreateDynamicMaterialInstance(7);
}

float ABossCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	currentHP -= DamageAmount;

	APlayerCharacter* Causer = Cast<APlayerCharacter>(DamageCauser);

	if (isCoolTime)
	{
		HitYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Causer->GetActorLocation()).Yaw;
		PlayAnimMontage(HitMontage, 1.2f, GetHitDirection(HitYaw));
	}

	UTargetWidget* TargetHpWidget = Cast<UTargetWidget>(TargetWidget->GetUserWidgetObject());

	if (TargetHpWidget == nullptr) return DamageAmount;
	TargetHpWidget->SetHPBarRatio(currentHP / maxHP);

	//위젯 보스 체력 바를 수정
	MainGM->SetBossHpRatio(currentHP / maxHP);

	if (Phase > 1)
	{
		SetPhaseMaterial();
	}

	if (currentHP <= 0.0f)
	{
		DropOrbsType = "red";
		OrbsAmount = 1000;
		OrbsSend(Causer);
		PlayAnimMontage(DeathMontage, 1.2f, "DeathStart");

		isDeath = true;
		isClear = true;
		if (LandingLocActor != nullptr)
		{
			LandingLocActor->Destroy();
		}

	}

	return 0.0f;
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (currentHP >= (maxHP * 0.7f))
	{
		Phase = 1;
	}
	else if (currentHP >= (maxHP * 0.3f))
	{
		Phase = 2;
	}
	else
	{
		Phase = 3;
	}

	BossPowerUp(Phase);

	if (isClear)
	{
		ClearTimer += DeltaTime;

		if (ClearTimer >= DestroyLimitTime)
		{
			isClear = false;
			MainGM->ShowAlertPopup("Game Clear! (Be Game data save)");
			MainGM->delBtnFun.AddLambda([this] {
				MainGM->GameProgressDataSave();
				MainGM->ShowMainLevel();
			});
		}
	}
}

// Called to bind functionality to input
void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABossCharacter::Attack()
{
	isBeingAttack = true;
	PlayAnimMontage(BossAttackMontage,
		MontagePlaySpeed,
		FName("a" + FString::FromInt((Phase == 3) ? 2 : Phase) +"_1"));
}

void ABossCharacter::NextAttack()
{
	PlayAttackNum++;
	PlayAnimMontage(BossAttackMontage,
		MontagePlaySpeed,
		FName("a" + FString::FromInt((Phase == 3) ? 2 : Phase) +"_" + FString::FromInt(PlayAttackNum)));
}

void ABossCharacter::ShotSpell()
{
	for (auto Spell : Spells)
	{
		IS_NULL(Spell);
		if (!Spell->IsActive())
		{
			Spell->SetActive(true);
			Spell->SetActorRelativeLocation(GetActorLocation() + GetActorForwardVector() + 100.f);
			Spell->FireInDirection(GetActorForwardVector());
		}
	}

	FTransform transform;
	transform.SetLocation(GetActorLocation() + GetActorForwardVector() * 100.f);

	ABossProjectileActor* obj = GetWorld()->SpawnActor<ABossProjectileActor>(ABossProjectileActor::StaticClass(), transform);

	IS_NULL(obj);
	obj->FireInDirection(GetActorForwardVector());
	obj->SetActive(true);

	Spells.Add(obj);
}

void ABossCharacter::SendDamage()
{
	//데미지 전달
	FVector start, end;
	start = GetActorLocation();
	end = start + (GetActorForwardVector() * attackRange);
	SendDamageToPlayer(start, end, EAttackType::SINGLE);
}

void ABossCharacter::RadialDamage()
{
	FVector s = GetActorLocation();
	FVector e = GetActorLocation();

	SendDamageToPlayer(s, e, EAttackType::MULTI);
}

void ABossCharacter::BossPowerUp(int32 n)
{
	float UpRate = 1.0f;
	switch (n)
	{
	case 1:
		UpRate = 1.2f;
		MontagePlaySpeed = 1.5f;
		LeapAttackCastingTime = 3.0f;
		break;
	case 2:
		MontagePlaySpeed = 2.0f;
		LeapAttackCastingTime = 2.0f;
		UpRate = 1.3f;
		break;
	case 3:
		MontagePlaySpeed = 3.0f;
		LeapAttackCastingTime = 1.5f;
		UpRate = 1.4f;
		break;
	}
	damage = DefaultDamage * UpRate;
	radialAttackRadius = DefaultRadialAttackRadius * UpRate;
	HadokenDamage = DefaultDamage * UpRate;
	GetCharacterMovement()->MaxWalkSpeed = DefaultMoveSpeed * UpRate;
	GetCharacterMovement()->JumpZVelocity = DefaultJumpVelocity * UpRate;
}
void ABossCharacter::SetLandingLocation(AActor* Target)
{
	FTransform transform;
	LandingLoc = (Target->GetActorForwardVector() * attackRange) + Target->GetActorLocation();

	LandingLoc.Z = GetCharacterMovement()->CurrentFloor.HitResult.Location.Z;
	//현재 액터의 위치 + 액터의 앞방향 * 100.f
	transform.SetLocation(LandingLoc);
	
	//ShotActor 생성
	LandingLocActor = GetWorld()->SpawnActor<ALandingMarkerActor>(ALandingMarkerActor::StaticClass(), transform);

}

void ABossCharacter::LeapAttackStart()
{
	if (!isBeingLeapAttack)
	{
		isBeingLeapAttack = true;
		FVector startLoc = GetActorLocation();      // 발사 지점

		float arcValue = 0.7f;
		FVector outVelocity = FVector::ZeroVector;   // 결과 Velocity

		if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, outVelocity, startLoc, LandingLoc, GetWorld()->GetGravityZ(), arcValue))
		{
			FPredictProjectilePathParams predictParams(20.0f, startLoc, outVelocity, 15.0f);   // 20: tracing 보여질 프로젝타일 크기, 15: 시물레이션되는 Max 시간(초)
			//predictParams.DrawDebugTime = 15.0f;     //디버그 라인 보여지는 시간 (초)
			//predictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;  // DrawDebugTime 을 지정하면 EDrawDebugTrace::Type::ForDuration 필요.
			predictParams.OverrideGravityZ = GetWorld()->GetGravityZ();
			FPredictProjectilePathResult result;
			UGameplayStatics::PredictProjectilePath(this, predictParams, result);
			LaunchCharacter(outVelocity, false, false);
		}

		PlayAnimMontage(LeapMontage, MontagePlaySpeed, "GroundSmashStart");
	}
}

void ABossCharacter::LeapAttackLoopEnd()
{
	PlayAnimMontage(LeapMontage, MontagePlaySpeed, "GroundSmashEnd");
}

void ABossCharacter::SetPhaseMaterial()
{
	float Enrage = (Phase < 3) ? 5.0f : 6.0f;
	float NonEnragedScale = (Phase < 3) ? 4.0f : 5.0f;
	float Speed = (Phase < 3) ? 10.0f : 15.0f;
	float MicroWaveSpeed = (Phase < 3) ? 10.f : 20.0f;
	PhaseMaterials->SetScalarParameterValue("Enrage", Enrage);
	PhaseMaterials->SetScalarParameterValue("NonEnragedScale", NonEnragedScale);
	PhaseMaterials->SetScalarParameterValue("Speed", Speed);
	PhaseMaterials->SetScalarParameterValue("MicroWaveSpeed", MicroWaveSpeed);
}
