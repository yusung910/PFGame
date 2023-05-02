// Fill out your copyright notice in the Description page of Project Settings.
#include "UnrealPortfolio.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "StartGameMode.h"

#include "MainGameInstance.h"
#include "StartGameMode.h"
#include "MainWidget.h"

void UMainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//�Լ��� ����() �ϴ°��� �ƴ϶� �Լ��� �ּҰ��� �����ִ°��� �Լ� ���� () ������
	btnNewGame->OnClicked.AddDynamic(this, &UMainWidget::OnClickNewGame);
	btnExit->OnClicked.AddDynamic(this, &UMainWidget::OnClickExit);
	btnLoad->OnClicked.AddDynamic(this, &UMainWidget::OnClickLoad);

	gi = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
	StartGM = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void UMainWidget::OnClickNewGame()
{
	//���� ���� ����
	StartGM->RemoveMainWidget();
	UGameplayStatics::OpenLevel(this, TEXT("Dungeon_1"));
}

void UMainWidget::OnClickLoad()
{
	StartGM->RemoveMainWidget();
	StartGM->DoLoadData();
}

void UMainWidget::OnClickExit()
{
	UKismetSystemLibrary::QuitGame(this, 0, EQuitPreference::Quit, false);
}

void UMainWidget::SetLoadBtnVisiblity(bool flag)
{
	btnLoad->SetVisibility((flag) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}


