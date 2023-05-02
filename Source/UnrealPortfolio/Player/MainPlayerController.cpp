// Fill out your copyright notice in the Description page of Project Settings.
//
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "UnrealPortfolio.h"

#include "PlayerEnumActor.h"
#include "PlayerCharacter.h"
#include "MainPlayerController.h"
#include "MainGameMode.h"

#include "MainGameInstance.h"


AMainPlayerController::AMainPlayerController()
{
	//틱함수실행!
	PrimaryActorTick.bCanEverTick = true;
}

void AMainPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	IS_NULL(InPawn);

	MyPlayer = Cast<APlayerCharacter>(InPawn);
	gi = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
	MainGM = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis(TEXT("MoveForward"), this, &AMainPlayerController::MoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &AMainPlayerController::MoveRight);
	InputComponent->BindAxis(TEXT("Zoom"), this, &AMainPlayerController::Zoom);
	InputComponent->BindAxis(TEXT("Turn"), this, &AMainPlayerController::Turn);
	InputComponent->BindAxis(TEXT("Lookup"), this, &AMainPlayerController::Lookup);

	//점프
	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMainPlayerController::PlayerJump);

	//근접 공격
	InputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AMainPlayerController::PlayerAttackPressed);

	InputComponent->BindAction(TEXT("Attack"), IE_Released, this, &AMainPlayerController::PlayerAttackReleased);

	//권총 샷
	InputComponent->BindAction(TEXT("HandGun"), IE_Pressed, this, &AMainPlayerController::PlayerSetHandGun);

	//타게팅
	InputComponent->BindAction(TEXT("Targeting"), IE_Pressed, this, &AMainPlayerController::GetTraceTarget);

	//언 타게팅
	InputComponent->BindAction(TEXT("Targeting"), IE_Released, this, &AMainPlayerController::SetUnTarget);

	//닷지 Dodge
	InputComponent->BindAction(TEXT("Dodge"), IE_Released, this, &AMainPlayerController::PlayerDodge);
	
	//SubAttack
	InputComponent->BindAction(TEXT("SubAttack"), IE_Released, this, &AMainPlayerController::PlayerSubAttack);

	//Show store
	InputComponent->BindAction(TEXT("ShowStore"), IE_Released, this, &AMainPlayerController::ShowStore);

	//BlockStart
	InputComponent->BindAction(TEXT("ShowStore"), IE_Pressed, this, &AMainPlayerController::BlockStart);


	//PushEscButton
	InputComponent->BindAction(TEXT("PushEscButton"), IE_Released, this, &AMainPlayerController::PushEscButton);

	//
	InputComponent->BindAction(TEXT("UltimateSkill"), IE_Released, this, &AMainPlayerController::UltimateSkill);

	InputComponent->BindAction(TEXT("Menu"), IE_Released, this, &AMainPlayerController::ShowMenu);

}

void AMainPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	//키 다운 체크
	fInputFlag.isSPress = IsInputKeyDown(EKeys::S);
	fInputFlag.isWPress = IsInputKeyDown(EKeys::W);
	fInputFlag.isAPress = IsInputKeyDown(EKeys::A);
	fInputFlag.isDPress = IsInputKeyDown(EKeys::D);
	fInputFlag.isFPress = IsInputKeyDown(EKeys::F);

	fInputFlag.isLeftClickDown = IsInputKeyDown(EKeys::LeftMouseButton);
	fInputFlag.isRightClickDown = IsInputKeyDown(EKeys::RightMouseButton);
	fInputFlag.isSpacePress = IsInputKeyDown(EKeys::SpaceBar);
	fInputFlag.isShiftPress = IsInputKeyDown(EKeys::LeftShift);

	fInputFlag.isSDown = WasInputKeyJustPressed(EKeys::S);
	fInputFlag.isWDown = WasInputKeyJustPressed(EKeys::W);
	fInputFlag.isADown = WasInputKeyJustPressed(EKeys::A);
	fInputFlag.isDDown = WasInputKeyJustPressed(EKeys::D);


	if (fInputFlag.isLeftClickDown && !fInputFlag.isRightClickDown)
	{
		timer = timer + GetWorld()->GetTimeSeconds();
		fInputFlag.isLeftCharge = (timer >= chargeTime) ? true : false;
	}
	else if (!fInputFlag.isLeftClickDown && fInputFlag.isRightClickDown)
	{
		timer = timer + GetWorld()->GetTimeSeconds();
		fInputFlag.isRightCharge = (timer >= chargeTime) ? true : false;
	}
	else
	{
		fInputFlag.isLeftCharge = false;
		fInputFlag.isRightCharge = false;
		timer = 0;
	}

}

void AMainPlayerController::MoveForward(float value)
{
	//키보드 W, S 축 매핑 함수
	if (value != 0.0f && MyPlayer->GetIsMove())
	{
		// 플레이어가 타겟을 락온 했을 때는 플레이어의 Rotation에 맞춰서 움직인다
		// 플레이어가 타겟이 락온하지 않았을 때는 스프링암(springArm)의 로테이션에 맞춰 움직인다.
		const FRotator Rotation = MyPlayer->GetSpringRot();

		const FRotator YawRotation(0, Rotation.Yaw, 0);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		MyPlayer->SetActorRotation(YawRotation);
		MyPlayer->AddMovementInput(Direction, value);
	}
}

void AMainPlayerController::MoveRight(float value)
{
	//키보드 A, D 축 매핑 함수
	if (value != 0.0f && MyPlayer->GetIsMove())
	{
		// 플레이어가 타겟을 락온 했을 때는 플레이어의 Rotation에 맞춰서 움직인다
		// 플레이어가 타겟이 락온하지 않았을 때는 스프링암(springArm)의 로테이션에 맞춰 움직인다.
		const FRotator Rotation = MyPlayer->GetSpringRot();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		MyPlayer->SetActorRotation(YawRotation);
		MyPlayer->AddMovementInput(Direction, value);
	}
}

void AMainPlayerController::Zoom(float value)
{
	//마우스 휠 스크롤 축 매핑 함수
	MyPlayer->Zoom(value);
}



void AMainPlayerController::Turn(float value)
{
	//마우스 X 이동 축 매핑 함수
	MyPlayer->SetSpringArmYaw(value);
}

void AMainPlayerController::Lookup(float value)
{
	//마우스 Y 이동 축 매핑 함수
	MyPlayer->SetSpringArmPitch(value);
}

void AMainPlayerController::PlayerJump()
{
	if (!MyPlayer->GetIsAttack() && !MyPlayer->GetIsBlock() && !MyPlayer->GetIsDown())
	{
		MyPlayer->Jump();
	}
}

void AMainPlayerController::PlayerDodge()
{
	MyPlayer->Dodge();
}

void AMainPlayerController::PlayerAttackPressed()
{
	if (MyPlayer->GetIsDown()) return;
	//MyPlayer->SetCurrentWeaponType(EPlayerWeapon::SWORD);
	//여기서 1.2초 이상 실행 되면? 트루로 변수를 넣는다.
	fInputFlag.isLeftClickPressed = false;
	FTimerHandle WaitHandle;
	float WaitTime = 0.2f; //시간을 설정하고
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{

			fInputFlag.isLeftClickPressed = true;

		}), WaitTime, false); //반복도 여기서 추가 변수를 선언해 설정가능

}

void AMainPlayerController::PlayerAttackReleased()
{
	if (MyPlayer->GetIsDown()) return;
	MyPlayer->SetCurrentWeaponType(EPlayerWeapon::SWORD);
	//공격 함수 호출
	MyPlayer->Attack();
}

void AMainPlayerController::PlayerSubAttack()
{
	if (MyPlayer->GetIsDown()) return;
	//공격 함수 호출
	MyPlayer->SubAttack();
}


void AMainPlayerController::PlayerSetHandGun()
{
	if (MyPlayer->GetIsDown()) return;
	MyPlayer->SetIsDashAttack(false);
	MyPlayer->SetCurrentWeaponType(EPlayerWeapon::HANDGUN);
	//공격 함수 호출
	MyPlayer->Attack();
}


void AMainPlayerController::GetTraceTarget()
{
	//충돌 가능 오브젝드 유형
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	//오브젝트 타입 중 Pawn 채널을 PawnType변수에 넣어준다.
	TEnumAsByte<EObjectTypeQuery> PawnType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody);
	ObjectTypes.Add(PawnType);

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(MyPlayer); //충돌을 무시할 액터 배열에 자신(this)을 넣는다.
	//
	TArray<FHitResult> hitResults; //충돌 결과를 저장할 구조체

	if (UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), //월드 객체
		MyPlayer->K2_GetActorLocation(), //start
		MyPlayer->K2_GetActorLocation(), //end 
		MyPlayer->GetLockOnDistance(), //거리
		ObjectTypes, //충돌 객체 타입 목록
		false, //
		IgnoreActors,
		EDrawDebugTrace::None,
		hitResults,
		true,
		FLinearColor::Red,
		FLinearColor::Green, 0.5f))
	{
		//타겟을 설정
		MyPlayer->SetTarget(hitResults);
		MyPlayer->GetTargetEnemy();
	};

}

void AMainPlayerController::SetUnTarget()
{
	MyPlayer->UnTargetingEnemy();
}

void AMainPlayerController::ShowStore()
{
	MyPlayer->DoBlock("BlockEnd");
	if (MyPlayer->GetIsNearestNPC())
	{
		//show store!!
		MainGM->ShowStoreWidget();
		MyPlayer->SetIsUsingStore(true);
		//상점에 들어갈 경우 각성 상태를 해제한다.
		MyPlayer->SetIsUltimateStatus(false);
	}
}

void AMainPlayerController::BlockStart()
{
	MyPlayer->DoBlock("BlockStart");
}

void AMainPlayerController::PushEscButton()
{
	MainGM->PushTheEscButton();
}

void AMainPlayerController::UltimateSkill()
{
	MyPlayer->SetIsUltimateStatus(!MyPlayer->GetIsUltimateStatus());
}

void AMainPlayerController::ShowMenu()
{
	MainGM->ShowMenu();
}
