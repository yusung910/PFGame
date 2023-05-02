// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"

#include "Kismet/GameplayStatics.h"

#include "Components/BoxComponent.h"

#include "MainGameInstance.h"
#include "MainGameMode.h"

#include "WarfareTriggerVolActor.h"

// Sets default values
AWarfareTriggerVolActor::AWarfareTriggerVolActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));


	//box �ݸ����� ũ�� ����
	TriggerZone->SetBoxExtent(FVector(300, 300, 300));
	RootComponent = TriggerZone;


	WarpParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("NaviLaserParticleComp"));

	GetObjectAsset(WarpParticle, UParticleSystem, "ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/combat/P_LevelExit_Ice.P_LevelExit_Ice'");

	WarpParticleComp->SetTemplate(WarpParticle);
	WarpParticleComp->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AWarfareTriggerVolActor::BeginPlay()
{
	Super::BeginPlay();

	gi = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
	MainGM = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

}

// Called every frame
void AWarfareTriggerVolActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWarfareTriggerVolActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//���� �ʵ忡 �ִ� ��� ���͸� ���� �� �� �̵� �� �� �ְ� ���� �ۼ�
	if (gi->GetMonsterCount() == 0.f && OtherActor->ActorHasTag("Player"))
	{
		UGameplayStatics::OpenLevel(this, TEXT("FrozenCove"));
	}
	else
	{

		//�ܿ� ���� ���� ǥ��
	}
}

void AWarfareTriggerVolActor::NotifyActorEndOverlap(AActor* OtherActor)
{
}

