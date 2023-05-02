// Fill out your copyright notice in the Description page of Project Settings.
#include "UnrealPortfolio.h"
#include "AIController.h"

#include "BossCharacter.h"

#include "BTTask_BossFirstAttack.h"

UBTTask_BossFirstAttack::UBTTask_BossFirstAttack()
{
	NodeName = TEXT("FirstAttack");

	//TickTask �Լ��� �����
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_BossFirstAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	//���� ĳ������ ���� �Լ� ����
	Boss->Attack();

	return EBTNodeResult::InProgress;
}

void UBTTask_BossFirstAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!Boss->GetIsBeingAttack())
	{
		//Montage_GetIsStopped : ��Ÿ�ְ� �������� ���� ��ȯ
		//GetCurrentMontage : ���� ������� ��Ÿ��
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
