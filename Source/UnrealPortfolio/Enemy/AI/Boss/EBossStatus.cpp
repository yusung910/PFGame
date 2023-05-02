// Fill out your copyright notice in the Description page of Project Settings.


#include "EBossStatus.h"

// Sets default values
AEBossStatus::AEBossStatus()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEBossStatus::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEBossStatus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

