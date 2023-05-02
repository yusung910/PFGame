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

	//��Ƽ���� ���۽� ȣ��Ǵ� �Լ�
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	//��Ƽ���� ����� ȣ��Ǵ� �Լ�
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
