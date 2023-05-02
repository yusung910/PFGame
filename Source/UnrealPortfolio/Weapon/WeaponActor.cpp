// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FPlayerControllerInputFlag.h"
#include "MainPlayerController.h"
#include "UnrealPortfolio.h"

#include "WeaponActor.h"

AWeaponActor::AWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	RootComponent = Root;
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
}

bool AWeaponActor::SendDamage(AActor* DamagedActor, float dmg)
{
	return false;
}

// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AMainPlayerController* controller = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	//�����ڿ��ٰ� �����ϸ� ������Ʈ �����Ƿ� ����!
	//�ǽð����� �Է� �����ؾ���.
	fPlayerInput = controller->GetControllerInputFlag();
}