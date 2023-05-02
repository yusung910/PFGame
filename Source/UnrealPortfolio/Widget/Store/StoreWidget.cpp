// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"

#include "MainGameInstance.h"
#include "Kismet/GameplayStatics.h"

#include "OrbsManagerComponent.h"
#include "OrbsEntry.h"
#include "MainGameMode.h"

#include "PlayerCharacter.h"

#include "StoreWidget.h"

UStoreWidget::UStoreWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//���� �÷��̾� ȭ���� widget�� ȣ���Ѵ�.
	GetClassAsset(OrbsEntryAsset, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Widgets/WB_OrbsEntry.WB_OrbsEntry_C'");
	GetClassAsset(WeaponUpgradeList, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Widgets/Store/WB_WeaponUpgradeList.WB_WeaponUpgradeList_C'");
	GetClassAsset(StatusUpgradeList, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Widgets/Store/WB_StatusUpgradeList.WB_StatusUpgradeList_C'");
	GetClassAsset(EtcList, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Widgets/Store/WB_ETCList.WB_ETCList_C'");
}

void UStoreWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//�Լ��� ����() �ϴ°��� �ƴ϶� �Լ��� �ּҰ��� �����ִ°��� �Լ� ���� () ������
	btnClose->OnClicked.AddDynamic(this, &UStoreWidget::CloseStoreWidget);
	btnStatus->OnClicked.AddDynamic(this, &UStoreWidget::ShowStatusUpgradeList);
	btnWeaponList->OnClicked.AddDynamic(this, &UStoreWidget::ShowWeaponUpgradeList);
	btnEtc->OnClicked.AddDynamic(this, &UStoreWidget::ShowETCList);

	gi = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
	MainGM = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	InitOrbsWidget(Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetOrbsComponent());
}

void UStoreWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->WasInputKeyJustReleased(EKeys::Escape))
	{
		CloseStoreWidget();
	}
}

void UStoreWidget::CloseStoreWidget()
{
	MainGM->PushTheEscButton();
}

void UStoreWidget::InitOrbsWidget(UOrbsManagerComponent* comp)
{
	OrbsVerticalContainer->ClearChildren();

	TArray<FOrbsDynamicData> datas = comp->GetOrbsDatas();

	OrbsEntryWidget = CreateWidget<UUserWidget>(GetWorld(), OrbsEntryAsset);
	OrbsVerticalContainer->AddChild(OrbsEntryWidget);

	Cast<UOrbsEntry>(OrbsEntryWidget)->InitOrbsData(comp, datas[0], 0);
}

void UStoreWidget::ShowWeaponUpgradeList()
{
	//���� ���׷��̵� ����� ȣ��
	VB_UpgradeList->ClearChildren();

	WeaponUpgradeWidget = CreateWidget<UUserWidget>(GetWorld(), WeaponUpgradeList);
	VB_UpgradeList->AddChild(WeaponUpgradeWidget);
}

void UStoreWidget::ShowStatusUpgradeList()
{
	//�ɷ�ġ ���׷��̵� ����� ȣ��
	VB_UpgradeList->ClearChildren();

	StatusUpgradeWidget = CreateWidget<UUserWidget>(GetWorld(), StatusUpgradeList);
	VB_UpgradeList->AddChild(StatusUpgradeWidget);
}

void UStoreWidget::ShowETCList()
{
	//�ɷ�ġ ���׷��̵� ����� ȣ��
	VB_UpgradeList->ClearChildren();

	EtcWidget = CreateWidget<UUserWidget>(GetWorld(), EtcList);
	VB_UpgradeList->AddChild(EtcWidget);
}
