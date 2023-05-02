// Fill out your copyright notice in the Description page of Project Settings.

#include "BossAIController.h"
#include "BlackboardKeys.h"

#include "UnrealPortfolio.h"
#include "BossCharacter.h"
#include "PlayerCharacter.h"

#include "Kismet/KismetMathLibrary.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


ABossAIController::ABossAIController()
{
	//틱 함수 사용 여부 false: 사용안함, true: 사용
	PrimaryActorTick.bCanEverTick = true;

	//AIPerceptinoComponent 생성
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception")));

	//Blackboard Component 생성
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

	GetObjectAsset(BTree, UBehaviorTree, "BehaviorTree'/Game/Blueprints/AI/BT_Boss.BT_Boss'");

	//SightConfig Component 생성
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->SightRadius = 2500.f;//볼 수 있는 거리
	SightConfig->LoseSightRadius = 3000.f;//본 목표물을 볼 수 있는 거리
	SightConfig->PeripheralVisionAngleDegrees = 350.f;//시야각도
	SightConfig->SetMaxAge(15.f); //볼수 있는 시간
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; //중립 인지

	GetPerceptionComponent()->ConfigureSense(*SightConfig); //감지 정보를 시야로 등록
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation()); //감지우선순위를 시야로 등록

	//감지 정보가 바뀌게 되면 실행 하게되는 이벤트
	//자극에 정보가 변경되었다면 OnTargetDetected() 호출
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ABossAIController::OnTargetDetected);

}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//BehaviorTree 시작
	if (BTree == nullptr) return;
	RunBehaviorTree(BTree);

	//BehaviorTree에 연결 된 블랙보드를 BlackboardComponent에 넣어준다.
	if (Blackboard == nullptr) return;
	Blackboard->InitializeBlackboard(*BTree->BlackboardAsset);

	IS_NULL(InPawn);
	Boss = Cast<ABossCharacter>(InPawn);

	GetBlackboardComponent()->SetValueAsInt(BBKeys::PatternPhase, Boss->GetBossPhase());
}

void ABossAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Boss->GetIsDeath())
	{
		UnPossess();
	}
}

void ABossAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	//AI가 인지한 액터가 플레이어 캐릭터라면
	if (auto const Player = Cast<APlayerCharacter>(Actor))
	{
		//Blackboard의 isFindPlayer키에 AI가 감지한 결과(bool) 데이터를 넣는다
		GetBlackboardComponent()->SetValueAsBool(BBKeys::isFindPlayer, Stimulus.WasSuccessfullySensed());

		isFindPlayer = true;

		TSubclassOf<UAISense> AISense;
		TArray<AActor*> Actors;

		//현재 감지된 액터를 배열로 반환.
		GetPerceptionComponent()->GetCurrentlyPerceivedActors(AISense, Actors);

		//해당 액터 목록을 배열로 반복한다.
		for (auto Other : Actors)
		{
			//플레이어 캐릭터와 일치하는 other의 객체를 찾는다.
			if (auto target = Cast<APlayerCharacter>(Other))
			{
				//찾은 target 객체를 블랙보드의 target에 할당한다.
				GetBlackboardComponent()->SetValueAsObject(BBKeys::target, target);
				FRotator rot = UKismetMathLibrary::FindLookAtRotation(Boss->GetActorLocation(), target->GetActorLocation());

				Boss->SetActorRotation(FRotator(0, rot.Yaw, 0));
			}
		}
	}
}
