// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"
#include "Components/WidgetComponent.h"
#include "CharacterCommonWidget.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacter.h"
#include "OrbsManagerComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "CommonCharacter.h"

// Sets default values
ACommonCharacter::ACommonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//���ʹ� ��ü�� ���߿� �� ���� ��� �ٸ� ĳ���͵��� �Ʒ��� ��������
	Land = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Land"));
	Land->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	Land->SetRelativeScale3D(FVector(5.0f, 5.0f, 100.f));
	//Land->bHiddenInGame = false;
	Land->SetupAttachment(GetMesh());
	Land->SetCollisionProfileName(FName("IgnoreOnlyPawn"));
	//�ش� ĸ�� ������Ʈ ���� �ٸ� ĳ���Ͱ� �ö� �� �� �ִ���?
	Land->CanCharacterStepUpOn = ECB_No;
	Land->SetWalkableSlopeOverride(FWalkableSlopeOverride(EWalkableSlopeBehavior::WalkableSlope_Unwalkable, 90.f));

	//���� ���� ���� ����Ʈ
	//���̾ư��� ������Ʈ ����
	UNiagaraSystem* sys1;
	GetNiagaraComp(HitBlood1, sys1, GetMesh(), "HitBlood1", "NiagaraSystem'/Game/FXVillesBloodVFXPack/Particles/Niagara/Toon/NS_Styleblood_Slash_Z_LRG.NS_Styleblood_Slash_Z_LRG'");
}

// Called when the game starts or when spawned
void ACommonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACommonCharacter::MovingActor(FVector s, FVector e, bool& flag, float& rate, float increaseRate)
{
	if (flag)
	{
		if (rate <= 1.f)
		{
			rate += increaseRate;
			FVector loc = UKismetMathLibrary::VLerp(s, e, rate);
			SetActorLocation(loc);
		}
		else
		{
			flag = false;
			rate = 0.0f;
		}
	}
}

void ACommonCharacter::SendDamageToPlayer(FVector TraceStart, FVector TraceEnd, EAttackType attackType)
{
	//�浹���� ������Ʈ ����
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	TEnumAsByte<EObjectTypeQuery> PawnType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody);	//��Ÿ��

	ObjectTypes.Add(PawnType);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);	//������ ���� �迭
	FHitResult hitResult; //sinlge
	TArray<FHitResult> hitResults; //multi

	bool bIsLocalTrace = false;

	switch (attackType)
	{
	case EAttackType::SINGLE:
		bIsLocalTrace = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, attackRadius,
			ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, hitResult, true,
			FLinearColor::Red, FLinearColor::Green, 0.0f);

		if (bIsLocalTrace)
		{
			if (hitResult.GetActor()->ActorHasTag(FName(TEXT("Player"))))
			{
				UGameplayStatics::ApplyDamage(hitResult.GetActor(), damage, GetController(), this, UDamageType::StaticClass());
			}
		}

		break;
	case EAttackType::MULTI:
		bIsLocalTrace = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), TraceStart, TraceEnd, radialAttackRadius,
			ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, hitResults, true,
			FLinearColor::Red, FLinearColor::Green, 0.0f);

		if (bIsLocalTrace)
		{

			TArray<ACommonCharacter*> DamagedList; //targeting ��ü ���
			for (auto hitR : hitResults)
			{
				ACommonCharacter* hitActor = Cast<ACommonCharacter>(hitR.GetActor());
				if (hitActor != nullptr && hitR.GetActor()->ActorHasTag(FName(TEXT("Player"))))
				{
					DamagedList.AddUnique(Cast<ACommonCharacter>(hitR.GetActor()));
				}
			}

			for (auto actor : DamagedList)
			{
				if (!actor->GetIsDown())
				{
					APlayerCharacter* player = Cast<APlayerCharacter>(actor);
					player->SetBeDamagedKnockback(true);
					if (!player->GetIsBlock())
					{
						player->SetIsDown(true);
					}
				}

				UGameplayStatics::ApplyDamage(actor, damage, GetController(), this, UDamageType::StaticClass());
			}
		}

		break;
	}
}

float ACommonCharacter::GetCharacterCalculateDirection(const FVector Velocity, const FRotator BaseRotation)
{
	if (!Velocity.IsNearlyZero()) //�ӷ��� 0�� �ƴҰ��
	{
		//���޹��� Rotation�� FVector::ZeroVector(���κ���)�� �̿��Ͽ� ���(Matrix)�� ��ȯ
		//���޹��� Rotation���� �������� �� �࿡ ���� �󸶳� ȸ���Ǿ� �ִ��� ��ȯ
		FMatrix RotMatrix = FRotationMatrix(BaseRotation); 

		//����� RotMatrix���� X���� ������ ������ �����´�
		FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
		//����� RotMatrix���� Y���� ������ ������ �����´�
		FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
		//�ӵ��� Normal(��������)�� �����´�
		FVector NormalizedVel = Velocity.GetSafeNormal2D();

		// �� ������ �������� ���Ѵ�.
		float ForwardCosAngle = FVector::DotProduct(ForwardVector, NormalizedVel);
		// ������ ���� ���� ��ȯ�Ѵ�.(Radians�� Degree��)
		float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

		// ȸ�� �迭(RotMatrix)���� Y���� ������ ���� ���� '�ӷ��� ��������(NormalizedVel)'�� �������� �����´�
		float RightCosAngle = FVector::DotProduct(RightVector, NormalizedVel);

		if (RightCosAngle < 0)
		{
			ForwardDeltaDegree *= -1;
		}

		return ForwardDeltaDegree;
	}

	return 0.f;
}

// Called every frame
void ACommonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (currentHP <= 0.0f && !isDeath)
	{
		DestroyTimeAccumulate = 0.0f;
	}

	if (currentHP <= 0.0f && isBeingKnockBack && !isDeath)
	{
		//ü���� 0 �����̸鼭, �˹� ���ϴ� ���̰�, ���� �ʾ��� ���
		//�˹� ���� �� ��� ����
		if (GetCharacterMovement()->CurrentFloor.IsWalkableFloor())
		{
			isDeath = true;
		}
	}

	if (isDeath)
	{
		DestroyTimeAccumulate += DeltaTime;
		if (DestroyTimeAccumulate >= DestroyLimitTime)
		{
			Destroy();
		}
	}
}

// Called to bind functionality to input
void ACommonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACommonCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	PlayHittedEffect();
	return DamageAmount;
}

void ACommonCharacter::SetVisiblityTargetWidget(bool isShow)
{
	TargetWidget->SetVisibility(isShow);
}

void ACommonCharacter::SetGravity(float val)
{
	GetCharacterMovement()->GravityScale = val;
}

FName ACommonCharacter::GetHitDirection(float HitYaw)
{
	FName HitPlayMontageName;

	//��Ʈ Animation ��Ÿ�� ���
	if (HitYaw >= 0.f && HitYaw < 90.f)
	{
		HitPlayMontageName = FName("HitFront");
	}
	else if (HitYaw >= 90.f && HitYaw < 180.f)
	{
		HitPlayMontageName = FName("HitRight");
	}
	else if (HitYaw >= 180.f && HitYaw < 270.f)
	{
		HitPlayMontageName = FName("HitBack");
	}
	else if (HitYaw >= 270.f && HitYaw <= 360.f)
	{
		HitPlayMontageName = FName("HitLeft");
	}
	return HitPlayMontageName;
}

void ACommonCharacter::CharacterDeath(UAnimMontage* deathMontage)
{
	PlayAnimMontage(deathMontage, 1.2f);
	GetController()->UnPossess();
}

void ACommonCharacter::KnockBackCharacter(FVector dir)
{
	FVector launchVec = dir * 1000.f;
	launchVec.Z += 500.f;
	SetGravity(3.0f);
	isBeingKnockBack = true;
	LaunchCharacter(launchVec, false, false);
}

void ACommonCharacter::OrbsSend(AActor* receiveActor)
{
	//orbsComp ȣ��
	UActorComponent* OrbsComp = GetComponentByClass(UOrbsManagerComponent::StaticClass());

	if (OrbsComp)
	{
		TArray<FOrbsSendData> sendOrbs;

		FOrbsSendData sendOrb;
		sendOrb.ScalePercentage = { 50.0f, 0.0f, 0.0f, 0.0f, 50.0f };
		sendOrb.Type = DropOrbsType;
		sendOrb.Amount = OrbsAmount;
		sendOrb.Scale = EOrbScale::FIFTEEN;
		sendOrb.bInfinitySend = false;
		sendOrb.bDynamicDecrease = false;
		sendOrbs.Add(sendOrb);

		Cast<UOrbsManagerComponent>(OrbsComp)->SendOrbsMultiData(receiveActor, sendOrbs);
	}

	isOrbsDrop = true;
}


void ACommonCharacter::PlayHittedEffect()
{
	HitBlood1->SetWorldLocation(GetActorLocation());
	HitBlood1->SetVisibility(true);
	HitBlood1->ActivateSystem();
}
