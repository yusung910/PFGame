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
	//ƽ �Լ� ��� ���� false: ������, true: ���
	PrimaryActorTick.bCanEverTick = true;

	//AIPerceptinoComponent ����
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception")));

	//Blackboard Component ����
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

	GetObjectAsset(BTree, UBehaviorTree, "BehaviorTree'/Game/Blueprints/AI/BT_Boss.BT_Boss'");

	//SightConfig Component ����
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->SightRadius = 2500.f;//�� �� �ִ� �Ÿ�
	SightConfig->LoseSightRadius = 3000.f;//�� ��ǥ���� �� �� �ִ� �Ÿ�
	SightConfig->PeripheralVisionAngleDegrees = 350.f;//�þ߰���
	SightConfig->SetMaxAge(15.f); //���� �ִ� �ð�
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; //�߸� ����

	GetPerceptionComponent()->ConfigureSense(*SightConfig); //���� ������ �þ߷� ���
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation()); //�����켱������ �þ߷� ���

	//���� ������ �ٲ�� �Ǹ� ���� �ϰԵǴ� �̺�Ʈ
	//�ڱؿ� ������ ����Ǿ��ٸ� OnTargetDetected() ȣ��
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ABossAIController::OnTargetDetected);

}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//BehaviorTree ����
	if (BTree == nullptr) return;
	RunBehaviorTree(BTree);

	//BehaviorTree�� ���� �� �����带 BlackboardComponent�� �־��ش�.
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
	//AI�� ������ ���Ͱ� �÷��̾� ĳ���Ͷ��
	if (auto const Player = Cast<APlayerCharacter>(Actor))
	{
		//Blackboard�� isFindPlayerŰ�� AI�� ������ ���(bool) �����͸� �ִ´�
		GetBlackboardComponent()->SetValueAsBool(BBKeys::isFindPlayer, Stimulus.WasSuccessfullySensed());

		isFindPlayer = true;

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
				FRotator rot = UKismetMathLibrary::FindLookAtRotation(Boss->GetActorLocation(), target->GetActorLocation());

				Boss->SetActorRotation(FRotator(0, rot.Yaw, 0));
			}
		}
	}
}
