// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "WeaponActor.h"

#include "EnableComboANS.h"

void UEnableComboANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp == nullptr) return;
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(MeshComp->GetOwner());

	if (MyPlayer == nullptr) return;
	MyPlayer->GetCurrentWeaponObj()->SetIsEnableCombo(true); //���� ���� ���� ����
}

void UEnableComboANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp == nullptr) return;
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(MeshComp->GetOwner());

	if (MyPlayer == nullptr) return;
	MyPlayer->GetCurrentWeaponObj()->SetIsEnableCombo(false); //���� ���� ���� ����
}
