// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetStringLibrary.h"

#include "OrbsManagerComponent.h"

#include "OrbsEntry.h"

void UOrbsEntry::InitOrbsData(UOrbsManagerComponent* comp, FOrbsDynamicData data, int32 idx)
{
	OrbManagerComponent = comp;
	OrbDynamicData = data;
	OrbsDataIdx = idx;
	RefreshOrbData();
}

void UOrbsEntry::RefreshOrbData()
{
	IS_NULL(OrbManagerComponent);

	if (OrbManagerComponent->GetOrbsDatas().IsValidIndex(OrbsDataIdx))
	{
		OrbDynamicData = OrbManagerComponent->GetOrbsDatas()[OrbsDataIdx];
		OrbImageColor->SetColorAndOpacity(FLinearColor(OrbDynamicData.OrbsColor.R, OrbDynamicData.OrbsColor.G, OrbDynamicData.OrbsColor.R));
		//�����ϰ� �ִ� Orbs�� ��(Amount)�� String���� �ٲ۴�, �ٲܶ� ������ , �� ��ü�Ѵ�.
		FString cnt = FString::FromInt(OrbDynamicData.CurrentAmount);
		
		//FString value = UKismetStringLibrary::Replace(cnt, " ", ",");
		//FText tmpText = FText::FromString(cnt);
		FText tmpText = UKismetTextLibrary::Conv_IntToText(OrbDynamicData.CurrentAmount);

		OrbValue->SetText(tmpText);
	}
	
}
