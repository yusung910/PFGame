// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"

#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "BlackboardKeys.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


#include "EnemyAIController.h"

AEnemyAIController::AEnemyAIController()
{
	//틱 함수 사용 여부 false: 사용안함, true: 사용
	PrimaryActorTick.bCanEverTick = true;

	//AIPerceptinoComponent 생성
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception")));

	//Blackboard Component 생성
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

	GetObjectAsset(BTree, UBehaviorTree, "BehaviorTree'/Game/Blueprints/AI/BT_EnemyPt1.BT_EnemyPt1'");

	//SightConfig Component 생성
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	
	SightConfig->SightRadius = 700.f;//볼 수 있는 거리
	SightConfig->LoseSightRadius = 800.f;//본 목표물을 볼 수 있는 거리
	SightConfig->PeripheralVisionAngleDegrees = 90.f;//시야각도
	SightConfig->SetMaxAge(3.f); //볼수 있는 시간
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; //중립 인지

	GetPerceptionComponent()->ConfigureSense(*SightConfig); //감지 정보를 시야로 등록
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation()); //감지우선순위를 시야로 등록

	//감지 정보가 바뀌게 되면 실행 하게되는 이벤트
	//자극에 정보가 변경되었다면 OnTargetDetected() 호출
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);

}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//BehaviorTree 시작
	if (BTree == nullptr) return;
	RunBehaviorTree(BTree);

	//BehaviorTree에 연결 된 블랙보드를 BlackboardComponent에 넣어준다.
	if (Blackboard == nullptr) return;
	Blackboard->InitializeBlackboard(*BTree->BlackboardAsset);

	IS_NULL(InPawn);
	Enemy = Cast<AEnemyCharacter>(InPawn);
	
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	IS_NULL(Enemy);
	if (Enemy->GetIsDown())
	{
		DownTimer = 0.f;
		GetBlackboardComponent()->SetValueAsBool(BBKeys::isDown, true);
	}
	else
	{
		DownTimer += DeltaTime;
		if (DownTimer > 1.0f)
		{
			GetBlackboardComponent()->SetValueAsBool(BBKeys::isDown, false);
		}
	}

	//사망시 AI연결 해제
	if (Enemy->GetIsDeath())
	{
		UnPossess();
	}
}

void AEnemyAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	//AI가 인지한 액터가 플레이어 캐릭터라면
	if (auto const Player = Cast<APlayerCharacter>(Actor))
	{
		//Blackboard의 isFindPlayer키에 AI가 감지한 결과(bool) 데이터를 넣는다
		GetBlackboardComponent()->SetValueAsBool(BBKeys::isFindPlayer, Stimulus.WasSuccessfullySensed());

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
			}
		}
	}
}