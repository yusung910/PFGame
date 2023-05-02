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
	//ƽ �Լ� ��� ���� false: ������, true: ���
	PrimaryActorTick.bCanEverTick = true;

	//AIPerceptinoComponent ����
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception")));

	//Blackboard Component ����
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

	GetObjectAsset(BTree, UBehaviorTree, "BehaviorTree'/Game/Blueprints/AI/BT_EnemyPt1.BT_EnemyPt1'");

	//SightConfig Component ����
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	
	SightConfig->SightRadius = 700.f;//�� �� �ִ� �Ÿ�
	SightConfig->LoseSightRadius = 800.f;//�� ��ǥ���� �� �� �ִ� �Ÿ�
	SightConfig->PeripheralVisionAngleDegrees = 90.f;//�þ߰���
	SightConfig->SetMaxAge(3.f); //���� �ִ� �ð�
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; //�߸� ����

	GetPerceptionComponent()->ConfigureSense(*SightConfig); //���� ������ �þ߷� ���
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation()); //�����켱������ �þ߷� ���

	//���� ������ �ٲ�� �Ǹ� ���� �ϰԵǴ� �̺�Ʈ
	//�ڱؿ� ������ ����Ǿ��ٸ� OnTargetDetected() ȣ��
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);

}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//BehaviorTree ����
	if (BTree == nullptr) return;
	RunBehaviorTree(BTree);

	//BehaviorTree�� ���� �� �����带 BlackboardComponent�� �־��ش�.
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

	//����� AI���� ����
	if (Enemy->GetIsDeath())
	{
		UnPossess();
	}
}

void AEnemyAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	//AI�� ������ ���Ͱ� �÷��̾� ĳ���Ͷ��
	if (auto const Player = Cast<APlayerCharacter>(Actor))
	{
		//Blackboard�� isFindPlayerŰ�� AI�� ������ ���(bool) �����͸� �ִ´�
		GetBlackboardComponent()->SetValueAsBool(BBKeys::isFindPlayer, Stimulus.WasSuccessfullySensed());

		TSubclassOf<UAISense> AISense;
		TArray<AActor*> Actors;

		//���� ������ ���͸� �迭�� ��ȯ.
		GetPerceptionComponent()->GetCurrentlyPerceivedActors(AISense, Actors);

		//�ش� ���� ����� �迭�� �ݺ��Ѵ�.
		for (auto Other : Actors)
		{
			//�÷��̾� ĳ���Ϳ� ��ġ�ϴ� other�� ��ü�� ã�´�.
			if (auto target = Cast<APlayerCharacter>(Other))
			{
				//ã�� target ��ü�� �������� target�� �Ҵ��Ѵ�.
				GetBlackboardComponent()->SetValueAsObject(BBKeys::target, target);
			}
		}
	}
}