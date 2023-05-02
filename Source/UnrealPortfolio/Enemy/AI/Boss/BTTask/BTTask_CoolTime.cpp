// Fill out your copyright notice in the Description page of Project Settings.
#include "UnrealPortfolio.h"

#include "AIController.h"

#include "BossCharacter.h"
#include "BossAnimInstance.h"
#include "BossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "BlackboardKeys.h"

#include "BTTask_CoolTime.h"

UBTTask_CoolTime::UBTTask_CoolTime()
{
	NodeName = TEXT("CoolTime");
	//TickTask �Լ��� �����
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_CoolTime::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	switch (Boss->GetBossPhase())
	{
	case 1:
		fStartCoolTimeLimit = 2.0f;
		fEndCoolTimeLimit = 5.0f;
		break;
	case 2:
		fStartCoolTimeLimit = 1.5f;
		fEndCoolTimeLimit = 3.0f;
		break;
	case 3:
		fStartCoolTimeLimit = 1.0f;
		fEndCoolTimeLimit = 2.0f;
		break;
	}
	fCoolTimeLimit = UKismetMathLibrary::RandomFloatInRange(1.0f, 5.0f);
	fAccumulateTickTime = 0.0f;

	Boss->SetIsCoolTime(true);
	return EBTNodeResult::InProgress;
}

void UBTTask_CoolTime::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	fAccumulateTickTime += DeltaSeconds;

	if (fCoolTimeLimit <= fAccumulateTickTime)
	{
		//Montage_GetIsStopped : ��Ÿ�ְ� �������� ���� ��ȯ
		//GetCurrentMontage : ���� ������� ��Ÿ��
		Boss->SetIsCoolTime(false);
		uint8 nextExecuteKey = UKismetMathLibrary::RandomIntegerInRange(0, (uint8)EBossStatusKey::Avoid);

		BossAIController->GetBlackboardComponent()->SetValueAsEnum(BBKeys::BossStatusKey, (uint8)nextExecuteKey);

		EBTNodeResult::Type bAiEnd = (Boss->GetIsDeath()) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;

		FinishLatentTask(OwnerComp, bAiEnd);
	}
}
