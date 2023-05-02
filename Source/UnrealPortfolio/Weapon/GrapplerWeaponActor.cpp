// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"
#include "DrawDebugHelpers.h"

#include "Components/SphereComponent.h"
#include "Components/TimeLineComponent.h"

#include "PlayerCharacter.h"
#include "PlayerEnumActor.h"
#include "MainPlayerController.h"

#include "EnemyCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "GrapplerWeaponActor.h"


AGrapplerWeaponActor::AGrapplerWeaponActor()
{
	//������
	PrimaryActorTick.bCanEverTick = true;

	UStaticMesh* MeshAsset;
	GetObjectAsset(MeshAsset, UStaticMesh, "StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'");

	MyMesh->SetStaticMesh(MeshAsset);
	MyMesh->AttachTo(Root);

	MyMesh->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f));
	MyMesh->SetCollisionProfileName("PhysicsActor");

	//���̾ư��� ������Ʈ ����
	Web = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Web"));

	//UStaticMeshComponent ������Ʈ�� ���̾ư��� ������Ʈ�� �ٿ� �ִ´�.
	Web->SetupAttachment(MyMesh);

	//���̾ư��� �ý��� ������Ʈ�� ���̾ư��� ������Ʈ asset�� �ִ´�.

	//���̾ư��� �ý��� ������Ʈ�� �����´�.
	GetObjectAsset(NWebSystem, UNiagaraSystem, "NiagaraSystem'/Game/SpiderWeb/Niagara/P_SpiderShoot.P_SpiderShoot'");
	Web->SetAsset(NWebSystem);

	//�� ���͸� ���� ����ü�� ����
	WebProjectile = CreateDefaultSubobject<USphereComponent>(TEXT("WebProjectile"));

	//�� ���� ����ü�� Root Static mesh�� ���δ�.
	WebProjectile->AttachTo(Root);

	//������ �������� ����(ũ��)
	WebProjectile->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));


	//������ �浹 ���������� ����
	WebProjectile->SetCollisionProfileName("PhysicsActor");
	
	////�ش� ������Ʈ�� �������� �����Ұ�����. (single body)
	WebProjectile->SetSimulatePhysics(true);

	////�߷¿����� ���� �ʰ� ����
	WebProjectile->SetEnableGravity(false); 

	//ȭ�鿡 ǥ�� �Ұ�����
	WebProjectile->SetVisibility(true);

	MyMesh->SetVisibility(false);

	//�׷� �ִϸ��̼� ��Ÿ�ָ� ȣ��
	GetObjectAsset(GrapMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Player/Player_Grapple_Shoot_Montage.Player_Grapple_Shoot_Montage'");

	//�Ź��ٿ� � �ִϸ��̼��� �����ϱ����� curve ������ ȣ���Ѵ�.
	GetObjectAsset(CurveWebHit, UCurveFloat, "CurveFloat'/Game/SpiderWeb/curve/WebHit.WebHit'");

	//�� ������ ����Ʈ�� ������ �� ���� Ÿ�Ӷ��� Ŀ�� ������Ʈ�� ȣ��
	GetObjectAsset(CurveWebStopEffect, UCurveFloat, "CurveFloat'/Game/SpiderWeb/curve/StopEffect.StopEffect'");

	
}

void AGrapplerWeaponActor::BeginPlay()
{
	Super::BeginPlay();

	WebProjectile->bHiddenInGame = false;

	//���� ���� �� �Ź����� �����
	Web->SetFloatParameter("User.Stop", 1.0f);

	WebProjectile->SetRelativeLocation(Web->GetComponentLocation());

	if (CurveWebHit)
	{
		//Ÿ�Ӷ��� ����
		FOnTimelineFloat WebHitTimeLineProgress;
		//�ش� �Լ��� Ÿ�Ӷ��ο� ���ε��Ѵ�.
		WebHitTimeLineProgress.BindUFunction(this, FName("WebHitTimeLineProgress"));

		//CurveFloat���� �Ű������� �ؼ� �޾Ƴ���.
		//Ÿ�Ӷ����� Ŀ�꿡 �Ǽ� �������Լ��� ����� �Լ��� �����Ѵ�.
		WebHitTimeLine.AddInterpFloat(CurveWebHit, WebHitTimeLineProgress);
	}

	if (CurveWebStopEffect)
	{
		//Ÿ�Ӷ��� ����
		FOnTimelineFloat WebStopEffectTimeLineProgress;
		//�ش� �Լ��� Ÿ�Ӷ��ο� ���ε��Ѵ�.
		WebStopEffectTimeLineProgress.BindUFunction(this, FName("WebStopEffectTimeLineProgress"));

		StopEffectTimeLine.AddInterpFloat(CurveWebStopEffect, WebStopEffectTimeLineProgress);
	}

}

void AGrapplerWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(GetOwner());

	//�Ź��ٰ� ���õ� Ÿ�Ӷ����� ƽ������ ���� ��Ų��
	WebHitTimeLine.TickTimeline(DeltaTime);
	StopEffectTimeLine.TickTimeline(DeltaTime);
	
	if (isShootedWeb && MyPlayer->GetIsLockOnEnemy())
	{
		fWebVLerpRate += 0.08f;
		if (!isWebApproached)
		{
			MoveWeb(MyPlayer->GetActorLocation(), MyPlayer->GetLockEnemy()->GetActorLocation());
			if (fWebVLerpRate >= 1.0f)
			{
				isWebApproached = true;
				fWebVLerpRate = 0.0f;

				WebHitTimeLine.Play();
			}
		}

		if(isWebApproached && !isEnemyArrival)
		{
			//�߻�ü�� ����߰� Enemy Target�� ���� ��
			MoveWeb(MyPlayer->GetLockEnemy()->GetActorLocation(), MyPlayer->GetActorLocation());
			
			DoDragedEnemy();
			if (fWebVLerpRate >= 1.0f)
			{
				fWebVLerpRate = 0.0f;
				isEnemyArrival = false;
				isShootedWeb = false;
				isWebApproached = false;
				StopEffectTimeLine.Play();
				WebHitTimeLine.SetNewTime(0.f);
			}
		}
	}
}

void AGrapplerWeaponActor::MoveWeb(FVector s, FVector e)
{
	FVector ProjectileLoc = UKismetMathLibrary::VLerp(s, e, fWebVLerpRate);
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	//GetComponentLocation() : �������Ʈ�� GetWorldLocation() ó�� ���ȴ�.
	Web->SetVectorParameter("User.BeamEnd", ProjectileLoc);
	Web->SetFloatParameter("UserDistance", UKismetMathLibrary::Vector_Distance(s, e));
}

void AGrapplerWeaponActor::WebHitTimeLineProgress(float Value)
{
	Web->SetFloatParameter("User.Hit", Value);
}

void AGrapplerWeaponActor::WebStopEffectTimeLineProgress(float Value)
{
	Web->SetFloatParameter("User.Stop", Value);
}

void AGrapplerWeaponActor::DoDragedEnemy()
{
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(GetOwner());

	if (MyPlayer->GetLockEnemy())
	{
		FVector start, end;

		start = MyPlayer->GetActorLocation();
		end = MyPlayer->GetLockEnemy()->GetActorLocation();

		//������ ����
		AEnemyCharacter* enemy = Cast<AEnemyCharacter>(WebHitResult.GetActor());
		
		IS_NULL(enemy);

		//���� enemy ĳ���͸� ����´�.
		enemy->DoDraggedCharacter(end, ((MyPlayer->GetActorForwardVector()) * 200.f) + start);

		FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(enemy->GetActorLocation(), MyPlayer->GetActorLocation());
		enemy->SetActorRotation(FRotator(0, lookAt.Yaw, 0));
	}
}

void AGrapplerWeaponActor::Attack()
{
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(GetOwner());
	if (MyPlayer->GetIsLockOnEnemy() && !MyPlayer->GetIsbeingGrapped())
	{
		MyPlayer->SetIsMove(false);
		MyPlayer->SetIsbeingGrapped(true);

		FName MontageName;
		if (MyPlayer->GetIsAir())
		{
			MontageName = "GrapAir";
		}
		else
		{
			MontageName = "GrapLand";
		}

		MyPlayer->PlayAnimMontage(GrapMontage, attackSpeed, MontageName);

	}
}

void AGrapplerWeaponActor::SendDamage()
{
	//����ü�� �߻� �ߴ���
	isShootedWeb = true;
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(GetOwner());
	if (MyPlayer->GetIsLockOnEnemy())
	{
		//�Ź����� ���̰� ����
		Web->SetFloatParameter("User.Stop", 0.0f);

		//������ ����
		FVector start, end;

		start = MyPlayer->GetActorLocation();
		end = MyPlayer->GetLockEnemy()->GetActorLocation();

		//�浹 ���� ������Ʈ ����
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

		//������Ʈ Ÿ�� �� Pawn ä���� PawnType������ �־��ش�.
		TEnumAsByte<EObjectTypeQuery> PawnType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody);
		ObjectTypes.Add(PawnType);

		//������ ���� �迭
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(MyPlayer); //�浹�� ������ ���� �迭�� �ڽ�(this)�� �ִ´�.

		//�浹 ���
		FHitResult hitResult;

		if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), start, end, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, hitResult, true, FLinearColor::Red, FLinearColor::Green, 0.5f))
		{
			//����ü�� �浹 ���� ���Ϳ��� �߻�ü ��ġ(WorldLocation)�� �븻���� ���� ����.
			FVector MeshToImpactLoc = (hitResult.ImpactPoint - MyMesh->GetComponentLocation()).GetSafeNormal();
			

			WebHitResult = hitResult;
		}
	}
}


