// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include "BossCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h"

#include "BossProjectileActor.h"
// Sets default values
ABossProjectileActor::ABossProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));

	CollisionComponent->SetSphereRadius(SpellRadius);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionProfileName("OverlapAll");

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABossProjectileActor::OnTriggerEnter);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	ProjectileMovementComponent->bInitialVelocityInLocalSpace = false;
	ProjectileMovementComponent->InitialSpeed = 2000.f; //초기 속도
	ProjectileMovementComponent->MaxSpeed = 2000.f;  // 최대속도
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;//중력
	ProjectileMovementComponent->bRotationFollowsVelocity = true; //속도에 따른 회전


	SpellParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SpellParticleSystem"));

	UParticleSystem* particleAsset;
	GetObjectAsset(particleAsset, UParticleSystem, "ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/P_HeldCharge_Fire_02.P_HeldCharge_Fire_02'");

	SpellParticleSystem->SetTemplate(particleAsset);
	SpellParticleSystem->AttachTo(CollisionComponent);

}

void ABossProjectileActor::SetActive(bool InActive)
{
	SetActorTickEnabled(InActive);

	//충돌 활성화 -> true: 활성화, false: 비활성화
	SetActorEnableCollision(InActive);

	//액터 숨김 여부 -> true: 액터를 숨긴다, false: 액터를 표시한다.
	SetActorHiddenInGame(!InActive);

	bActive = InActive;

	//활성화 시킨다면
	if (InActive)
	{
		//5초 후 액터 비활성화
		//마지막 인자값 : 반복여부
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ABossProjectileActor::Deactivator, 5.f, false);
	}
}

void ABossProjectileActor::Deactivator()
{
	SetActive(false);
}

bool ABossProjectileActor::IsActive()
{
	return bActive;
}

void ABossProjectileActor::FireInDirection(const FVector& direction)
{
	ProjectileMovementComponent->Velocity = direction * ProjectileMovementComponent->InitialSpeed;
}

// Called when the game starts or when spawned
void ABossProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABossProjectileActor::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//충돌한 상대 액터의 태그가 Player를 지니고 있다면?
	if (OtherActor->ActorHasTag(FName(TEXT("Player"))))
	{
		ABossCharacter* Boss = Cast<ABossCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossCharacter::StaticClass()));
		UGameplayStatics::ApplyDamage(OtherActor, Boss->GetHadokenDamage(), Boss->GetController(), this, UDamageType::StaticClass());

		Destroy();
		SetActive(false);
	}
}

// Called every frame
void ABossProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

