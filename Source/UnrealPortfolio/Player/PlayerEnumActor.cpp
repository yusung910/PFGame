// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerEnumActor.h"

// Sets default values
APlayerEnumActor::APlayerEnumActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerEnumActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerEnumActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

