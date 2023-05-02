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

	//에너미 객체가 공중에 떠 있을 경우 다른 캐릭터들이 아래로 못들어가도록
	Land = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Land"));
	Land->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	Land->SetRelativeScale3D(FVector(5.0f, 5.0f, 100.f));
	//Land->bHiddenInGame = false;
	Land->SetupAttachment(GetMesh());
	Land->SetCollisionProfileName(FName("IgnoreOnlyPawn"));
	//해당 캡슐 컴포넌트 위에 다른 캐릭터가 올라 설 수 있는지?
	Land->CanCharacterStepUpOn = ECB_No;
	Land->SetWalkableSlopeOverride(FWalkableSlopeOverride(EWalkableSlopeBehavior::WalkableSlope_Unwalkable, 90.f));

	//히팅 관련 블러드 이펙트
	//나이아가라 컴포넌트 생성
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
	//충돌가능 오브젝트 유형
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	TEnumAsByte<EObjectTypeQuery> PawnType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody);	//폰타입

	ObjectTypes.Add(PawnType);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);	//무시할 액터 배열
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

			TArray<ACommonCharacter*> DamagedList; //targeting 객체 목록
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
	if (!Velocity.IsNearlyZero()) //속력이 0이 아닐경우
	{
		//전달받은 Rotation과 FVector::ZeroVector(제로벡터)를 이용하여 행렬(Matrix)로 변환
		//전달받은 Rotation값을 기준으로 각 축에 따라 얼마나 회전되어 있는지 반환
		FMatrix RotMatrix = FRotationMatrix(BaseRotation); 

		//변경된 RotMatrix에서 X축이 가지는 방향을 가져온다
		FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
		//변경된 RotMatrix에서 Y축이 가지는 방향을 가져온다
		FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
		//속도의 Normal(법선벡터)를 가져온다
		FVector NormalizedVel = Velocity.GetSafeNormal2D();

		// 두 벡터의 내적값을 구한다.
		float ForwardCosAngle = FVector::DotProduct(ForwardVector, NormalizedVel);
		// 구해진 각도 값을 변환한다.(Radians을 Degree로)
		float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

		// 회전 배열(RotMatrix)에서 Y축이 가지는 방향 값과 '속력의 법선벡터(NormalizedVel)'의 내적값을 가져온다
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
		//체력이 0 이하이면서, 넉백 당하는 중이고, 죽지 않았을 경우
		//넉백 종료 후 사망 판정
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

	//히트 Animation 몽타주 재생
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
	//orbsComp 호출
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
