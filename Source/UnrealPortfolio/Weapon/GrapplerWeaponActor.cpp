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
	//생성자
	PrimaryActorTick.bCanEverTick = true;

	UStaticMesh* MeshAsset;
	GetObjectAsset(MeshAsset, UStaticMesh, "StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'");

	MyMesh->SetStaticMesh(MeshAsset);
	MyMesh->AttachTo(Root);

	MyMesh->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f));
	MyMesh->SetCollisionProfileName("PhysicsActor");

	//나이아가라 컴포넌트 생성
	Web = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Web"));

	//UStaticMeshComponent 컴포넌트에 나이아가라 컴포넌트를 붙여 넣는다.
	Web->SetupAttachment(MyMesh);

	//나이아가라 시스템 오브젝트를 나이아가라 컴포넌트 asset에 넣는다.

	//나이아가라 시스템 오브젝트를 가져온다.
	GetObjectAsset(NWebSystem, UNiagaraSystem, "NiagaraSystem'/Game/SpiderWeb/Niagara/P_SpiderShoot.P_SpiderShoot'");
	Web->SetAsset(NWebSystem);

	//웹 슈터를 위한 투사체를 생성
	WebProjectile = CreateDefaultSubobject<USphereComponent>(TEXT("WebProjectile"));

	//웹 슈터 투사체를 Root Static mesh에 붙인다.
	WebProjectile->AttachTo(Root);

	//웹슈터 스케일을 설정(크기)
	WebProjectile->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));


	//웹슈터 충돌 프로파일을 성정
	WebProjectile->SetCollisionProfileName("PhysicsActor");
	
	////해당 컴포넌트에 물리학을 적용할것인지. (single body)
	WebProjectile->SetSimulatePhysics(true);

	////중력영향을 받지 않게 설정
	WebProjectile->SetEnableGravity(false); 

	//화면에 표현 할것인지
	WebProjectile->SetVisibility(true);

	MyMesh->SetVisibility(false);

	//그랩 애니메이션 몽타주를 호출
	GetObjectAsset(GrapMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Player/Player_Grapple_Shoot_Montage.Player_Grapple_Shoot_Montage'");

	//거미줄에 곡선 애니메이션을 적용하기위한 curve 파일을 호출한다.
	GetObjectAsset(CurveWebHit, UCurveFloat, "CurveFloat'/Game/SpiderWeb/curve/WebHit.WebHit'");

	//웹 슈터의 이펙트를 종료할 때 사용될 타임라인 커브 오브젝트를 호출
	GetObjectAsset(CurveWebStopEffect, UCurveFloat, "CurveFloat'/Game/SpiderWeb/curve/StopEffect.StopEffect'");

	
}

void AGrapplerWeaponActor::BeginPlay()
{
	Super::BeginPlay();

	WebProjectile->bHiddenInGame = false;

	//게임 시작 시 거미줄을 숨긴다
	Web->SetFloatParameter("User.Stop", 1.0f);

	WebProjectile->SetRelativeLocation(Web->GetComponentLocation());

	if (CurveWebHit)
	{
		//타임라인 생성
		FOnTimelineFloat WebHitTimeLineProgress;
		//해당 함수를 타임라인에 바인딩한다.
		WebHitTimeLineProgress.BindUFunction(this, FName("WebHitTimeLineProgress"));

		//CurveFloat값을 매개변수로 해서 받아낸다.
		//타임라인의 커브에 실수 보간법함수에 실행될 함수를 지정한다.
		WebHitTimeLine.AddInterpFloat(CurveWebHit, WebHitTimeLineProgress);
	}

	if (CurveWebStopEffect)
	{
		//타임라인 생성
		FOnTimelineFloat WebStopEffectTimeLineProgress;
		//해당 함수를 타임라인에 바인딩한다.
		WebStopEffectTimeLineProgress.BindUFunction(this, FName("WebStopEffectTimeLineProgress"));

		StopEffectTimeLine.AddInterpFloat(CurveWebStopEffect, WebStopEffectTimeLineProgress);
	}

}

void AGrapplerWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(GetOwner());

	//거미줄과 관련된 타임라인을 틱단위로 실행 시킨다
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
			//발사체가 출발했고 Enemy Target에 가는 중
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
	//GetComponentLocation() : 블루프린트의 GetWorldLocation() 처럼 사용된다.
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

		//딜레이 설정
		AEnemyCharacter* enemy = Cast<AEnemyCharacter>(WebHitResult.GetActor());
		
		IS_NULL(enemy);

		//잡힌 enemy 캐릭터를 끌어온다.
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
	//투사체를 발사 했는지
	isShootedWeb = true;
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(GetOwner());
	if (MyPlayer->GetIsLockOnEnemy())
	{
		//거미줄을 보이게 설정
		Web->SetFloatParameter("User.Stop", 0.0f);

		//데미지 전달
		FVector start, end;

		start = MyPlayer->GetActorLocation();
		end = MyPlayer->GetLockEnemy()->GetActorLocation();

		//충돌 가능 오브젝트 유형
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

		//오브젝트 타입 중 Pawn 채널을 PawnType변수에 넣어준다.
		TEnumAsByte<EObjectTypeQuery> PawnType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody);
		ObjectTypes.Add(PawnType);

		//무시할 액터 배열
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(MyPlayer); //충돌을 무시할 액터 배열에 자신(this)을 넣는다.

		//충돌 결과
		FHitResult hitResult;

		if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), start, end, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, hitResult, true, FLinearColor::Red, FLinearColor::Green, 0.5f))
		{
			//투사체의 충돌 지점 벡터에서 발사체 위치(WorldLocation)의 노말벡터 값을 뺀다.
			FVector MeshToImpactLoc = (hitResult.ImpactPoint - MyMesh->GetComponentLocation()).GetSafeNormal();
			

			WebHitResult = hitResult;
		}
	}
}


