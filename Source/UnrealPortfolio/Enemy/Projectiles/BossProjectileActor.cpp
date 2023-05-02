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
	ProjectileMovementComponent->InitialSpeed = 2000.f; //�ʱ� �ӵ�
	ProjectileMovementComponent->MaxSpeed = 2000.f;  // �ִ�ӵ�
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;//�߷�
	ProjectileMovementComponent->bRotationFollowsVelocity = true; //�ӵ��� ���� ȸ��


	SpellParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SpellParticleSystem"));

	UParticleSystem* particleAsset;
	GetObjectAsset(particleAsset, UParticleSystem, "ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/P_HeldCharge_Fire_02.P_HeldCharge_Fire_02'");

	SpellParticleSystem->SetTemplate(particleAsset);
	SpellParticleSystem->AttachTo(CollisionComponent);

}

void ABossProjectileActor::SetActive(bool InActive)
{
	SetActorTickEnabled(InActive);

	//�浹 Ȱ��ȭ -> true: Ȱ��ȭ, false: ��Ȱ��ȭ
	SetActorEnableCollision(InActive);

	//���� ���� ���� -> true: ���͸� �����, false: ���͸� ǥ���Ѵ�.
	SetActorHiddenInGame(!InActive);

	bActive = InActive;

	//Ȱ��ȭ ��Ų�ٸ�
	if (InActive)
	{
		//5�� �� ���� ��Ȱ��ȭ
		//������ ���ڰ� : �ݺ�����
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
	//�浹�� ��� ������ �±װ� Player�� ���ϰ� �ִٸ�?
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

