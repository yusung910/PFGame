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
	//ƽ�Լ�����!
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

	//����
	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMainPlayerController::PlayerJump);

	//���� ����
	InputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AMainPlayerController::PlayerAttackPressed);

	InputComponent->BindAction(TEXT("Attack"), IE_Released, this, &AMainPlayerController::PlayerAttackReleased);

	//���� ��
	InputComponent->BindAction(TEXT("HandGun"), IE_Pressed, this, &AMainPlayerController::PlayerSetHandGun);

	//Ÿ����
	InputComponent->BindAction(TEXT("Targeting"), IE_Pressed, this, &AMainPlayerController::GetTraceTarget);

	//�� Ÿ����
	InputComponent->BindAction(TEXT("Targeting"), IE_Released, this, &AMainPlayerController::SetUnTarget);

	//���� Dodge
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
	//Ű �ٿ� üũ
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
	//Ű���� W, S �� ���� �Լ�
	if (value != 0.0f && MyPlayer->GetIsMove())
	{
		// �÷��̾ Ÿ���� ���� ���� ���� �÷��̾��� Rotation�� ���缭 �����δ�
		// �÷��̾ Ÿ���� �������� �ʾ��� ���� ��������(springArm)�� �����̼ǿ� ���� �����δ�.
		const FRotator Rotation = MyPlayer->GetSpringRot();

		const FRotator YawRotation(0, Rotation.Yaw, 0);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		MyPlayer->SetActorRotation(YawRotation);
		MyPlayer->AddMovementInput(Direction, value);
	}
}

void AMainPlayerController::MoveRight(float value)
{
	//Ű���� A, D �� ���� �Լ�
	if (value != 0.0f && MyPlayer->GetIsMove())
	{
		// �÷��̾ Ÿ���� ���� ���� ���� �÷��̾��� Rotation�� ���缭 �����δ�
		// �÷��̾ Ÿ���� �������� �ʾ��� ���� ��������(springArm)�� �����̼ǿ� ���� �����δ�.
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
	//���콺 �� ��ũ�� �� ���� �Լ�
	MyPlayer->Zoom(value);
}



void AMainPlayerController::Turn(float value)
{
	//���콺 X �̵� �� ���� �Լ�
	MyPlayer->SetSpringArmYaw(value);
}

void AMainPlayerController::Lookup(float value)
{
	//���콺 Y �̵� �� ���� �Լ�
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
	//���⼭ 1.2�� �̻� ���� �Ǹ�? Ʈ��� ������ �ִ´�.
	fInputFlag.isLeftClickPressed = false;
	FTimerHandle WaitHandle;
	float WaitTime = 0.2f; //�ð��� �����ϰ�
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{

			fInputFlag.isLeftClickPressed = true;

		}), WaitTime, false); //�ݺ��� ���⼭ �߰� ������ ������ ��������

}

void AMainPlayerController::PlayerAttackReleased()
{
	if (MyPlayer->GetIsDown()) return;
	MyPlayer->SetCurrentWeaponType(EPlayerWeapon::SWORD);
	//���� �Լ� ȣ��
	MyPlayer->Attack();
}

void AMainPlayerController::PlayerSubAttack()
{
	if (MyPlayer->GetIsDown()) return;
	//���� �Լ� ȣ��
	MyPlayer->SubAttack();
}


void AMainPlayerController::PlayerSetHandGun()
{
	if (MyPlayer->GetIsDown()) return;
	MyPlayer->SetIsDashAttack(false);
	MyPlayer->SetCurrentWeaponType(EPlayerWeapon::HANDGUN);
	//���� �Լ� ȣ��
	MyPlayer->Attack();
}


void AMainPlayerController::GetTraceTarget()
{
	//�浹 ���� �������� ����
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	//������Ʈ Ÿ�� �� Pawn ä���� PawnType������ �־��ش�.
	TEnumAsByte<EObjectTypeQuery> PawnType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody);
	ObjectTypes.Add(PawnType);

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(MyPlayer); //�浹�� ������ ���� �迭�� �ڽ�(this)�� �ִ´�.
	//
	TArray<FHitResult> hitResults; //�浹 ����� ������ ����ü

	if (UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), //���� ��ü
		MyPlayer->K2_GetActorLocation(), //start
		MyPlayer->K2_GetActorLocation(), //end 
		MyPlayer->GetLockOnDistance(), //�Ÿ�
		ObjectTypes, //�浹 ��ü Ÿ�� ���
		false, //
		IgnoreActors,
		EDrawDebugTrace::None,
		hitResults,
		true,
		FLinearColor::Red,
		FLinearColor::Green, 0.5f))
	{
		//Ÿ���� ����
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
		//������ �� ��� ���� ���¸� �����Ѵ�.
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
