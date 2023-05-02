// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"
#include "AIController.h"

#include "BossCharacter.h"
#include "BTTask_Hadouken.h"

UBTTask_Hadouken::UBTTask_Hadouken()
{
	NodeName = TEXT("Hadouken");

	//TickTask �Լ��� �����
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Hadouken::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	//���� ĳ������ ���� �Լ� ����
	Boss->Attack();

	//���� ĳ���Ͱ� �Ϲ� ���� ��ǿ� ���̾�� ���
	Boss->SetIsHadouKenAttack(true);

	return EBTNodeResult::InProgress;
}

void UBTTask_Hadouken::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!Boss->GetIsBeingAttack())
	{
		//Montage_GetIsStopped : ��Ÿ�ְ� �������� ���� ��ȯ
		//GetCurrentMontage : ���� ������� ��Ÿ��
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
