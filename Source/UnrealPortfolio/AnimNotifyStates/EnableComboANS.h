// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "EnableComboANS.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UEnableComboANS : public UAnimNotifyState
{
	GENERATED_BODY()
	
protected:

	//노티파이 시작시 호출되는 함수
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	//노티파이 종료시 호출되는 함수
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
