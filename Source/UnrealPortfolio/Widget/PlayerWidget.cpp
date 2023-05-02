// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"

#include "Animation/WidgetAnimation.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "OrbsManagerComponent.h"
#include "OrbsEntry.h"
#include "MainGameInstance.h"
#include "MainGameMode.h"
#include "PlayerCharacter.h"

#include "PlayerWidget.h"


UPlayerWidget::UPlayerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//게임 플레이어 화면의 widget을 호출한다.
	GetClassAsset(OrbsEntryAsset, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Widgets/WB_OrbsEntry.WB_OrbsEntry_C'");

}

void UPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UPlayerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	gi = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
	txtSwordLevel->SetText(UKismetTextLibrary::Conv_IntToText(gi->GetSwordLevel()));
	txtGunLevel->SetText(UKismetTextLibrary::Conv_IntToText(gi->GetGunLevel()));
	
	SetHpBar(gi->GetHpRatio());
	SetManaBar(gi->GetManaRatio());
}

void UPlayerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	fTickCnt += InDeltaTime;

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Enemy"), EnemyList);
	
	txtEnemyCount->SetText(UKismetTextLibrary::Conv_IntToText(EnemyList.Num()));

	gi->SetMonsterCount(EnemyList.Num());

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Boss"), BossList);
	if (BossList.Num() > 0)
	{
		BossHP->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPlayerWidget::InitOrbsWidget(UOrbsManagerComponent* comp)
{
	OrbsManagerComp = comp;
	InitOrbs();
}


void UPlayerWidget::InitOrbs()
{
	//VerticalBox에 있는 자식 객체(orb)목록를 초기화한다.
	OrbsVerticalContainer->ClearChildren();

	TArray<FOrbsDynamicData> datas = OrbsManagerComp->GetOrbsDatas();

	OrbsEntryWidget = CreateWidget<UUserWidget>(GetWorld(), OrbsEntryAsset);
	OrbsVerticalContainer->AddChild(OrbsEntryWidget);

	Cast<UOrbsEntry>(OrbsEntryWidget)->InitOrbsData(OrbsManagerComp, datas[0], 0);
}


void UPlayerWidget::UpdateOrbs()
{
	//
	IS_NULL(BgBorderOrbs);
	BgBorderOrbs->SetRenderOpacity(1.0f);
	
	//
	IS_NULL(OrbsVerticalContainer);
	for (int i=0; i < OrbsVerticalContainer->GetChildrenCount(); i++)
	{
		UWidget* children = OrbsVerticalContainer->GetChildAt(i);
		if (children)
		{
			Cast<UOrbsEntry>(children)->RefreshOrbData();
		}
	}
	
	IS_NULL(FadeAnim);
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{

		PlayAnimation(FadeAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f, false);
	
	}), 1.0f, true); //반복도 여기서 추가 변수를 선언해 설정가능


}

void UPlayerWidget::ToggleStoreUseCopy(bool flag)
{
	ESlateVisibility visibleFlag = (flag) ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

	infoUseStore->SetVisibility(visibleFlag);
}

void UPlayerWidget::SetHpBar(float ratio)
{
	HpBar->SetPercent(ratio);
}

void UPlayerWidget::SetManaBar(float ratio)
{
	ManaBar->SetPercent(ratio);
}

void UPlayerWidget::SetBossHpbar(float ratio)
{
	BossHP->SetPercent(ratio);
}
