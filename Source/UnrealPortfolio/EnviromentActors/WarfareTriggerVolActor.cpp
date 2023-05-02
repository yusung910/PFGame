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


	//box 콜리전의 크기 변경
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
	//현재 필드에 있는 모든 몬스터를 제거 한 후 이동 할 수 있게 로직 작성
	if (gi->GetMonsterCount() == 0.f && OtherActor->ActorHasTag("Player"))
	{
		UGameplayStatics::OpenLevel(this, TEXT("FrozenCove"));
	}
	else
	{

		//잔여 몬스터 제거 표시
	}
}

void AWarfareTriggerVolActor::NotifyActorEndOverlap(AActor* OtherActor)
{
}

