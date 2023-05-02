// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"

#include "OrbsManagerComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DrawDebugHelpers.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"

#include "Components/CapsuleComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "PlayerAnimInstance.h"
#include "PlayerEnumActor.h"
#include "PlayerWidget.h"
#include "PlayerCameraShake.h"
#include "Particles/ParticleSystemComponent.h"


#include "WeaponActor.h"
#include "SwordWeaponActor.h"
#include "HandGunWeaponActor.h"
#include "GrapplerWeaponActor.h"

#include "EnemyCharacter.h"
#include "PlayerCharacter.h"


#include "MainGameInstance.h"
#include "MainGameMode.h"

#include "Math/Rotator.h"
#include "Animation/AnimInstance.h"

// Sets default values

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// * 메쉬 생성
	USkeletalMesh* mesh; 
	GetObjectAsset(mesh, USkeletalMesh, "SkeletalMesh'/Game/LordVictor/Mesh/LordVictor_Mobs.LordVictor_Mobs'");

	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->CanCharacterStepUpOn = ECB_No;

	//캐릭터가 컨트롤러의 회전에 영향을 받지 않음
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	AutoPossessPlayer = EAutoReceiveInput::Player0; //0번 플레이어

	GetCapsuleComponent()->SetCollisionProfileName(FName("PhysicsActor"));
	GetMesh()->SetCollisionProfileName(FName("PhysicsActor"));

	//피직스 핸들 생성
	physicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("physicsHandle"));

	physicsHandle->LinearDamping = 200.f;
	physicsHandle->LinearStiffness = 750.f;
	physicsHandle->AngularDamping = 500.f;
	physicsHandle->AngularStiffness = 1500.f;
	physicsHandle->InterpolationSpeed = 50.f;

	// * Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	//스프링암의 회전이 캐릭터나 컨트롤러가 아닌 월드 좌표에 따라 회전
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 20.f));

	// 스프링 암의 길이
	SpringArm->TargetArmLength = springArmLengthDefault;

	//충돌시 카메라 위치가 당겨지지 않도록(충돌 활성화)
	SpringArm->bDoCollisionTest = false;

	//SpringArm의 회전
	SpringArm->SetRelativeRotation(springRot);

	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;

	// * Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false; //pawn의 회전의 영향을 받지 않음.
	Camera->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	Camera->SetFieldOfView(110.f);

	// * 기타 설정
	GetCharacterMovement()->GravityScale = fGravityLimit; //중력
	GetCharacterMovement()->JumpZVelocity = fJumpVelocity; //캐릭터 점프 높이

	//최대 점프 횟수
	JumpMaxCount = 2;

	Tags.Add(FName(TEXT("Player"))); // 플레이어 태그 추가

	// * 애니메이션 생성
	TSubclassOf<UAnimInstance> anim;
	GetClassAsset(anim, UAnimInstance, "AnimBlueprint'/Game/Blueprints/Anim/Player/ABP_Player.ABP_Player_C'");
	GetMesh()->AnimClass = anim;

	// 점프 몽타주 생성
	GetObjectAsset(AdditionJumpMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Player/Double_Jump_Montage.Double_Jump_Montage'");

	GetObjectAsset(GunHitMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Player/GunHit/HandGun_Hit_Montage.HandGun_Hit_Montage'");

	GetObjectAsset(SwordHitMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Player/SwordHit/PlayerSwordHit.PlayerSwordHit'");

	GetObjectAsset(SwordBlockHitMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Player/SwordHit/Block_Hit_Seq_Montage.Block_Hit_Seq_Montage'");

	GetObjectAsset(DownMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Player/Hit_Down_Montage.Hit_Down_Montage'");


	GetObjectAsset(BlockMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Player/Block_Seq_Montage.Block_Seq_Montage'");

	//commonCharacter 클래스에 있는 체공 LaunchCharacter의 제한 높이다
	currentFlightHeight = GetFlightTimeLimit();

	//Orbs컴포넌트 생성
	OrbsComponent = CreateDefaultSubobject<UOrbsManagerComponent>(TEXT("OrbsComponent"));

	//플레이어 위젯 블루프린트 호출
	GetClassAsset(PlayerHud, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Widgets/BP_PlayerWidget.BP_PlayerWidget_C'");

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	this->CallWeaponActorAndAttach<ASwordWeaponActor>("SwordHand", "Sword");
	this->CallWeaponActorAndAttach<AHandGunWeaponActor>("HandGunSocket", "HandGun");
	this->CallWeaponActorAndAttach<AGrapplerWeaponActor>("GrapSocket", "Grap");

	Materials.Add(GetMesh()->CreateDynamicMaterialInstance(8));
	Materials.Add(GetMesh()->CreateDynamicMaterialInstance(9));
	Materials.Add(GetMesh()->CreateDynamicMaterialInstance(10));

	UltimateMaterials.Add(GetMesh()->CreateDynamicMaterialInstance(8));
	UltimateMaterials.Add(GetMesh()->CreateDynamicMaterialInstance(10));
	UltimateMaterials.Add(GetMesh()->CreateDynamicMaterialInstance(11));

	HpStatusMaterials.Add(GetMesh()->CreateDynamicMaterialInstance(8));
	HpStatusMaterials.Add(GetMesh()->CreateDynamicMaterialInstance(9));
	HpStatusMaterials.Add(GetMesh()->CreateDynamicMaterialInstance(10));
	HpStatusMaterials.Add(GetMesh()->CreateDynamicMaterialInstance(11));

	gi = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MainGM = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	//옷 색상 변환
	SetClothMaterialByHp(true);
}

float APlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float takeDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	isGotHit = true;

	//
	if (DamageCauser && !isDown)
	{
		HitYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DamageCauser->GetActorLocation()).Yaw;

		if (currentWeaponType == EPlayerWeapon::SWORD)
		{
			if (isBlock)
			{
				PlayAnimMontage(SwordBlockHitMontage, 1.2f);
			}
			else
			{
				PlayAnimMontage(SwordHitMontage, 1.2f, GetHitDirection(HitYaw));
			}
		}
		else if (currentWeaponType == EPlayerWeapon::HANDGUN)
		{
			PlayAnimMontage(GunHitMontage, 1.2f, GetHitDirection(HitYaw));
		}

	}

	if (isBlock)
	{
		gi->AccumulateCurrentHp(-(takeDamage * 0.1f));
	}
	else
	{
		gi->AccumulateCurrentHp(-takeDamage);
	}

	MainGM->SetHpRatio();

	if (gi->GetCurrentHp() <= 0)
	{
		GetCharacterMovement()->StopMovementImmediately(); //이동 중지
		GetCharacterMovement()->DisableMovement(); //이동 비활성화

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//GetController()->UnPossess(); //컨트롤러와 연결  해제

		Destroy(); //액터 제거

		MainGM->ShowAlertPopup("Game Over, Retry?");
		//마우스 커서를 보여준다.

		//MainGM->delBtnFun.AddUObject(this, &APlayerCharacter::DeadAfter);
		MainGM->delBtnFun.AddLambda([this] {
			MainGM->DoPlayerDeadAfter();
		});
	}

	//옷 색상 변환
	SetClothMaterialByHp(true);

	if (beDamagedKnockback && !isBlock)
	{
		beDamagedKnockback = false;
		DownTimer = 0.0f;
		DoCameraShake();
		//피격시 다운
		Down();
	}

	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Falling;
	isAttack = false;

	return takeDamage; //데미지 값 반환
	
}

template<class T>
void APlayerCharacter::CallWeaponActorAndAttach(FName socketName, FName weaponName)
{
	T* weaponActorAsset = GetWorld()->SpawnActor<T>(T::StaticClass());
	weaponActorAsset->K2_AttachToComponent(GetMesh(), socketName, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
	weaponActorAsset->SetActorEnableCollision(false);
	weaponActorAsset->Tags.Add(weaponName);
	weaponActorAsset->SetActorHiddenInGame(true);
	//Actor의 Owner를 설정
	weaponActorAsset->SetOwner(this);

	if (socketName == "GrapSocket")
	{
		grap = Cast<AGrapplerWeaponActor>(weaponActorAsset);
		subWeapon = grap;
		grap->SetActorHiddenInGame(false);
	}
	else
	{
		weaponActorAsset->Tags.Add("PlayerWeapon");
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LookAtTarget();
	Timer = Timer + DeltaTime; //타이머를 누적
	isAir = GetMovementComponent()->IsFalling();

	//Animation에 있는 함수를 확인한 결과!
	Direction = GetCharacterCalculateDirection(GetVelocity(), GetActorRotation());
	
	isFalling = (GetVelocity().Z > 0) ? false : true;
	
	if (!isAir)
	{
		currentFlightHeight = GetFlightTimeLimit();
		isATFAttack = false;
	}

	if (currentWeaponType == EPlayerWeapon::SWORD)
	{
		if (isDodge || isDashAttack || isFloorToAirAttack || isATFAttack)
		{
			float amount = UKismetMathLibrary::VSizeSquared(FVector(GetVelocity())) / 4000.0f;
			for (auto mat : Materials)
			{
				//뒷 방향 벡터
				mat->SetVectorParameterValue("Loc", (GetVelocity().GetSafeNormal()) * -1.0f);
				mat->SetScalarParameterValue("Amount", amount);
				mat->SetVectorParameterValue("SmearColor", FLinearColor(0.18f, 0.015f, 0.024f));
			}
		}
		else
		{
			for (auto mat : Materials)
			{
				mat->SetScalarParameterValue("Amount", 0.f);
			}
		}
	}

	if (isUltimateStatus && gi->GetCurrentMana() > 0)
	{
		float UltimateAmount = 0.05f;
		for (auto mat : UltimateMaterials)
		{
			//뒷 방향 벡터
			FLinearColor smearColor = FLinearColor(0.f, 0.f, 255.f);
			mat->SetVectorParameterValue("Loc", GetActorUpVector());
			mat->SetScalarParameterValue("Amount", UltimateAmount);
			mat->SetScalarParameterValue("Power", -2.f);
			mat->SetVectorParameterValue("SmearColor", FLinearColor(0.f, 0.f, 255.f));
		}
				
		gi->AccumulateCurrentMana(-DeltaTime);

		MainGM->SetManaRatio();

		MoveSpeed *= 1.09f;
	}
	else
	{
		UltimateTimer = 0.f;
		for (auto mat : UltimateMaterials)
		{
			mat->SetScalarParameterValue("Amount", 0.f);
			mat->SetVectorParameterValue("SmearColor", FLinearColor(0.f, 0.f, 0.f));
		}
		MoveSpeed = DefaultMoveSpeed;
		isUltimateStatus = false;
	}

	gi->SetWeaponDamage(isUltimateStatus);

	NPCList.Empty();
	//NPC 캐릭터가 플레이어와의 거리를 확인한다.
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("NPC"), NPCList);

	if (NPCList.Num() > 0)
	{
		for (AActor* npc  : NPCList)
		{
			float dist = UKismetMathLibrary::Vector_Distance(GetActorLocation(), npc->GetActorLocation());

			isNearestNPC = (dist < fDistScopeNPC);

			TArray<UUserWidget*> PlayerHudWidgets;

			UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), PlayerHudWidgets, PlayerHud);

			//월드에 추가의 위젯이 있는지 확인한다.
			if (PlayerHudWidgets.Num() > 0)
			{
				Cast<UPlayerWidget>(PlayerHudWidgets[0])->ToggleStoreUseCopy(isNearestNPC);
			}
		}
	}

	fCharMaxJump = GetCharacterMovement()->GetMaxJumpHeight() + GetActorLocation().Z;

	if (isDown)
	{
		DownTimer += DeltaTime;

		if (DownTimer >= DownDurationTime && !isBeingGetUp)
		{
			isBeingGetUp = true;
			GetUpCharacter();
		}
	}

	isCurrentAir = !GetCharacterMovement()->CurrentFloor.bWalkableFloor;
}

void APlayerCharacter::SetClothMaterialByHp(bool isDmg)
{
	//true일 경우 옷에 붉은색을 표현
	//false일 경우 옷을 원래색으로 돌린다.
	if (isDmg)
	{
		int32 rndMaterialNum = UKismetMathLibrary::RandomIntegerInRange(0, HpStatusMaterials.Num() - 1);
		FLinearColor mixColor = UKismetMathLibrary::VLerp(FVector(255.f, 0, 0), FVector(0, 0, 0), gi->GetHpRatio());
		HpStatusMaterials[rndMaterialNum]->SetVectorParameterValue("MixColor", mixColor);
	}
	else
	{
		for (int i=0; i< HpStatusMaterials.Num(); i++)
		{
			HpStatusMaterials[i]->SetVectorParameterValue("MixColor", FLinearColor(0,0,0));
		}
	}
}

void APlayerCharacter::DoCameraShake()
{
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(UPlayerCameraShake::StaticClass());
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCharacter::CheckJumpInput(float DeltaTime)
{
	Super::CheckJumpInput(DeltaTime);
}


void APlayerCharacter::Zoom(float value)
{
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength - (value * 10), 100.f, 600.f);
}

void APlayerCharacter::Jump()
{
	if (GetIsLockOnEnemy())
	{
		if ((Direction > -70.0f && Direction < 70.f) ||
			(Direction <= -160.0f || Direction >= 160.f))
		{
			DoDblJump();
		}
		
		if (isAir)
		{
			DoDblJump();
		}
	}
	else
	{
		DoDblJump();
	}
}

void APlayerCharacter::DoDblJump()
{
	Super::Jump();

	isAdditionJump = (JumpCurrentCount > 0 && JumpCurrentCount < JumpMaxCount) ? true : false;

	if (isAdditionJump)
	{
		PlayAnimMontage(AdditionJumpMontage, 1.2f);
		isAdditionJump = false;
	}
}

void APlayerCharacter::Dodge()
{
	if (!isDodge && GetIsLockOnEnemy() &&
		!GetVelocity().IsNearlyZero() &&
		!isAir &&
		(
			(Direction >= 70.f && Direction < 160.f ) ||
			(Direction <= -70.f && Direction > -160.f )
		)
		)
	{
		IS_NULL(currentWeapon);
		currentWeapon->Dodge();
	}
}

void APlayerCharacter::LookAtTarget()
{
	IS_NULL(lockEnemy);
	float TargetDistance = lockEnemy->GetDistanceTo(this);
	FRotator cameraRot = FRotator(0.f, 0.f, 0.f);
	FVector springLoc = GetActorLocation();
	float setSpringArmLength = 0.f;
	float currentSpringArmLength = SpringArm->TargetArmLength;

	if (TargetDistance <= lockOnDistance &&
		!lockEnemy->GetIsDeath() &&
		lockEnemy->GetCurrentHP() > 0.0f)
	{
		//타겟을 바라보게 한다.
		FVector playerLoc = GetActorLocation();
		FVector enemyLoc = lockEnemy->GetActorLocation();
		FVector playerForwardLoc = GetActorForwardVector();
		FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(playerLoc, enemyLoc);

		//총을 들고 있을 경우 Pitch값이 필요함.
		LookAtPitch = FMath::RoundHalfFromZero(lookAt.Pitch);
		
		if (!isDown)
		{
			//타겟을 바라볼 때 Yaw(도리도리)만 적용한다.
			SetActorRotation(FRotator(0, lookAt.Yaw, 0));
		}

		//Enemy 객체의 위치와 player의 위치를 모두 보여줄 수 있는 vector값을 찾아야한다.
		//enemy의 위치와 player의 위치 중간 값을 그린다
		playerToTargetCenter = ((playerLoc / 2) + (enemyLoc / 2));

		//스프링암의 위치를 중간지점으로 옮긴다.

		if (lockEnemy->ActorHasTag("Boss"))
		{
			setSpringArmLength = (GetDistanceTo(lockEnemy) / 2) + (springArmLengthDefault + 150.f);
		}
		else
		{
			setSpringArmLength = (GetDistanceTo(lockEnemy) / 2) + springArmLengthDefault;
		}



		springLoc = playerToTargetCenter;
	}
	else
	{
		GetTargetEnemy();
		setSpringArmLength = springArmLengthDefault;
	}

	SpringArm->TargetArmLength = setSpringArmLength;
	SpringArm->SetWorldLocation(springLoc);
}



void APlayerCharacter::SetTarget(TArray<FHitResult> hitResults)
{
	targetList.Empty();

	for (auto& hitTarget : hitResults)
	{
		targetList.AddUnique(Cast<ACommonCharacter>(hitTarget.GetActor()));
	}
}

void APlayerCharacter::GetTargetEnemy()
{
	ACommonCharacter* frontTarget = nullptr;
	
	float dist = lockOnDistance;

	if (targetList.Num() > 0)
	{
		for (auto& target : targetList)
		{
			//위젯을 모두 숨긴다.
			IS_NULL(target);
			target->SetVisiblityTargetWidget(false);
			float localDist = target->GetDistanceTo(this);
		
			//플레이어에서부터 가장 가까운 Enemy 객체를 설정
			if (dist > localDist && target->GetCurrentHP() >= 0.0f)
			{
				dist = localDist;
				frontTarget = Cast<ACommonCharacter>(target);
			}
		}

		IS_NULL(frontTarget);
		lockEnemy = frontTarget;
		lockEnemy->SetVisiblityTargetWidget(true);
	}

}

void APlayerCharacter::UnTargetingEnemy()
{
	if (lockEnemy != nullptr)
	{
		lockEnemy->SetVisiblityTargetWidget(false);

		lockEnemy = nullptr;
		targetList.Empty();

		SpringArm->SetWorldLocation(GetActorLocation());
	}
}

void APlayerCharacter::DoGrap()
{
	IS_NULL(subWeapon);
	subWeapon->SendDamage();
}


void APlayerCharacter::DoBlock(FName montageSecNm)
{
	if (!isDown)
	{
		SetCurrentWeaponType(EPlayerWeapon::SWORD);
		currentWeapon->ResetAttackCount();
		PlayAnimMontage(BlockMontage, 1.2f, montageSecNm);
	}
}

void APlayerCharacter::SetCurrentWeaponType(EPlayerWeapon val)
{
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PlayerWeapon"), WeaponList);

	//플레이어가 장착하고 있는 모든 무기 액터를 숨긴다.
	for (AActor* actor : WeaponList)
	{
		actor->SetActorHiddenInGame(true);
	}

	for (AActor* weaponActor : WeaponList)
	{
		Cast<AWeaponActor>(weaponActor)->SetIsLevitationAttack(false);

		if (weaponActor->ActorHasTag("Sword") && val == EPlayerWeapon::SWORD)
		{
			weaponActor->SetActorHiddenInGame(false);
			currentWeapon = Cast<ASwordWeaponActor>(weaponActor);
		}
		else if (weaponActor->ActorHasTag("HandGun") && val == EPlayerWeapon::HANDGUN)
		{
			weaponActor->SetActorHiddenInGame(false);
			currentWeapon = Cast<AHandGunWeaponActor>(weaponActor);
		}
	}
	currentWeaponType = val;

}

void APlayerCharacter::Attack()
{
	isDodge = false;
	isDashAttack = false;
	isFloorToAirAttack = false;
	isATFAttack = false;

	IS_NULL(currentWeapon);
	if (isBlock) return;
	currentWeapon->Attack();
}

void APlayerCharacter::Down()
{
	isDodge = false;
	isDashAttack = false;
	isFloorToAirAttack = false;
	isATFAttack = false;
	isMove = false;
	
	PlayAnimMontage(DownMontage, 1.2f, "DownStart");
}

void APlayerCharacter::GetUpCharacter()
{
	PlayAnimMontage(DownMontage, 1.2f, "DownToGetup");
}


void APlayerCharacter::SubAttack()
{
	isDodge = false;
	isDashAttack = false;
	isFloorToAirAttack = false;
	isATFAttack = false;

	IS_NULL(subWeapon);
	subWeapon->Attack();
}

void APlayerCharacter::SetSpringArmYaw(float val)
{
	springRot.Yaw = FMath::Fmod(springRot.Yaw + val, 360.f);
	SpringArm->SetRelativeRotation(springRot);
}

void APlayerCharacter::SetSpringArmPitch(float val)
{
	springRot.Pitch = FMath::Clamp(springRot.Pitch + val, -91.f, 1.f);
	SpringArm->SetRelativeRotation(springRot);
}

bool APlayerCharacter::GetIsAirPosition()
{
	return isCurrentAir;
}

void APlayerCharacter::DoAirLaunchCharacter()
{
	//공중 일 때만 실행
	if (isAir)
	{
		//공중 일 경우 체공 제한 시간을 초기화한다.
		currentFlightHeight = GetFlightTimeLimit();
		LaunchCharacter(FVector(0, 0, currentFlightHeight), true, true);
	}
}

void APlayerCharacter::DeadAfter()
{
	
}



