// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h"

#include "NavigationLaserActor.h"

// Sets default values
ANavigationLaserActor::ANavigationLaserActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));

	//UStaticMesh* mesh;
	//GetObjectAsset(mesh, UStaticMesh, "StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'");
	//MyMesh->SetStaticMesh(mesh);

	//RootComponent = MyMesh;

	NaviLaserParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("NaviLaserParticleComp"));

	GetObjectAsset(NaviLaserParticle, UParticleSystem, "ParticleSystem'/Game/NavigationLaser/mat/P_Laser.P_Laser'");

	NaviLaserParticleComp->SetTemplate(NaviLaserParticle);
	NaviLaserParticleComp->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void ANavigationLaserActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANavigationLaserActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANavigationLaserActor::SetLaserPointDirection(FVector s, FVector e)
{
	NaviLaserParticleComp->SetBeamSourcePoint(0, s, 0);
	NaviLaserParticleComp->SetBeamTargetPoint(0, e, 0);
}

