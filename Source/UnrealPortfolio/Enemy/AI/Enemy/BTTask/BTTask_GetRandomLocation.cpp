// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"
#include "BlackboardKeys.h"
#include "NavigationSystem.h"

#include "BTTask_GetRandomLocation.h"

UBTTask_GetRandomLocation::UBTTask_GetRandomLocation()
{
	NodeName = TEXT("GetRandomLocation");
}


EBTNodeResult::Type UBTTask_GetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (EnemyController == nullptr) return EBTNodeResult::Failed;
	APawn* Enemy = EnemyController->GetPawn();

	if (Enemy == nullptr) return EBTNodeResult::Failed;
	FVector Origin = Enemy->GetActorLocation(); //Enemy ���� ��ġ
	FNavLocation NavPos; //�׺���̼� ��� ��ġ���� ����ü

	//���� ���忡 �ִ� �׺���̼� �ý����� �����´�
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	if (NavSys == nullptr) return EBTNodeResult::Failed;

	bool bLocalRndPosResult = NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, NavPos);
	//�׺���̼� ��� ������ġ�� NavPos�� �־��ش�
	if (bLocalRndPosResult)
	{
		//������ ��ġ�� �������� randomPos�� ����
		EnemyController->GetBlackboardComponent()->SetValueAsVector(BBKeys::randomPos, NavPos.Location);

		return EBTNodeResult::Succeeded; //������ȯ
	}
	return EBTNodeResult::Failed; //���й�ȯ
}
