// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MoveStop.h"

#include "AIController.h"

#include "EnemyAnimInstance.h"
#include "EnemyCharacter.h"

UBTTask_MoveStop::UBTTask_MoveStop()
{
	NodeName = TEXT("MoveStop");

	//TickTask 함수가 실행됨
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveStop::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;
	if (OwnerComp.GetAIOwner()->GetPawn() == nullptr) return EBTNodeResult::Failed;

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	Enemy->StopAnimMontage(); //애님 몽타주 재생 정지
	Enemy->ImmediatelyStopMove(); //움직임을 정지

	return EBTNodeResult::InProgress;
}

void UBTTask_MoveStop::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetAIOwner() == nullptr) return;
	if (OwnerComp.GetAIOwner()->GetPawn() == nullptr) return;

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	UEnemyAnimInstance* Anim = Cast<UEnemyAnimInstance>(Enemy->GetMesh()->GetAnimInstance());
	if (Anim == nullptr) return;
	if (Anim->Montage_GetIsStopped(Enemy->GetCurrentMontage()))
	{
		//Montage_GetIsStopped : 몽타주가 끝낫는지 여부 반환
		//GetCurrentMontage : 현재 재생중인 몽타주
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
