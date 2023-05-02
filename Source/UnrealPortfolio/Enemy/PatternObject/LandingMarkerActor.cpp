// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"
#include "BossCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "LandingMarkerActor.h"

// Sets default values
ALandingMarkerActor::ALandingMarkerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MarkerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));

	//box 콜리전의 크기 변경
	MarkerZone->SetBoxExtent(FVector(300, 300, 1));
	RootComponent = MarkerZone;

	MarkerZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MarkerZone->SetCollisionProfileName("OverlapAll");

	MarkerZone->OnComponentBeginOverlap.AddDynamic(this, &ALandingMarkerActor::OnTriggerEnter);

	MarkerParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MarkerParticleSystem"));

	UParticleSystem* particleAsset;

	GetObjectAsset(particleAsset, UParticleSystem, "ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/P_Circle_ChargeUp_Fire_Phase2.P_Circle_ChargeUp_Fire_Phase2'");

	MarkerParticleSystem->SetTemplate(particleAsset);
	MarkerParticleSystem->AttachTo(MarkerZone);

}
// Called when the game starts or when spawned
void ALandingMarkerActor::BeginPlay()
{
	Super::BeginPlay();
}

void ALandingMarkerActor::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//충돌한 상대 액터의 태그가 Player를 지니고 있다면?
	if (OtherActor->ActorHasTag(FName(TEXT("Boss"))))
	{
		Destroy();
	}
}

// Called every frame
void ALandingMarkerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

