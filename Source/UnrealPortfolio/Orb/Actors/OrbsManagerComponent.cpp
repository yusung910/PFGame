// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPortfolio.h"
#include "OrbStructActor.h"

#include "MainGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "PlayerWidget.h"
#include "StoreWidget.h"

#include "OrbsSpawnerActor.h"
#include "OrbsManagerComponent.h"
#include "OrbsProjectilesBaseActor.h"

// Sets default values for this component's properties
UOrbsManagerComponent::UOrbsManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//오브 관련 프리셋 테이블 데이터 호출
	GetObjectAsset(OrbsPresetTable, UDataTable, "DataTable'/Game/DataTables/OrbPresetDataTable.OrbPresetDataTable'");

	//플레이어 위젯 블루프린트 호출
	GetClassAsset(PlayerHud, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Widgets/BP_PlayerWidget.BP_PlayerWidget_C'");

	GetClassAsset(StoreHud, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Widgets/Store/WB_Store.WB_Store_C'");
	
	DefaultSpawnerClass = Cast<AOrbsSpawnerActor>(AOrbsSpawnerActor::StaticClass()->GetDefaultObject());

	DefaultProjectilesClass = Cast<AOrbsProjectilesBaseActor>(AOrbsProjectilesBaseActor::StaticClass()->GetDefaultObject());

	LatestSpawnerClass = Cast<AOrbsSpawnerActor>(AOrbsSpawnerActor::StaticClass()->GetDefaultObject());
}



// Called when the game starts
void UOrbsManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	//게임 인스턴스를 호출한다
	gi = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());

	//데이터 테이블(OrbPresetDataTable)에 존재하는 오브 설정 정보를 호출한다.
	LoadOrbs();

	if (GetOwner()->ActorHasTag("Player"))
	{
		initialValues[0] = gi->GetAmount();
	}

	for (int i=0; i< OrbsList.Num(); i++)
	{
		SetOrbs(OrbsList[i], initialValues[i], EOrbScale::ONE, false, false);
	}

	APawn* Owner = Cast<APawn>(GetOwner());
	if (Owner)
	{
		if (Owner->IsPlayerControlled())
		{
			if (EnablePlayerWidget)
			{
				//Orb관련 위젯 생성
				OrbsSpawnWidget();
			}
		};
	
	}

	

	//델리게이트 생성
	EdOrbSetValue.AddDynamic(this, &UOrbsManagerComponent::BeSetOrbs);
	EdOrbSendData.AddDynamic(this, &UOrbsManagerComponent::BeSendOrbs);
	EdOrbReceiveData.AddDynamic(this, &UOrbsManagerComponent::BeReceivedOrbs);
	EdOrbSendSingle.AddDynamic(this, &UOrbsManagerComponent::BeSendSingleOrbs);
	EdOrbReceiveSingle.AddDynamic(this, &UOrbsManagerComponent::BeReceiveSingleOrbs);

}

void UOrbsManagerComponent::GetOrbs(FName type, bool& retVal, int32& foundIdx, FOrbsDynamicData& orbsData)
{
	int32 idx = OrbsList.Find(type);
	if (idx >= 0)
	{
		orbsData = OrbsDatas[idx];
	}
	retVal = (idx >= 0) ? true : false;
	foundIdx = idx;
}

void UOrbsManagerComponent::LoadOrbs()
{
	//프리셋데이터 전부를 호출한다.
	TArray<FName> OrbPresets = OrbsPresetTable->GetRowNames();
	for (FName data : OrbPresets)
	{
		FOrbsTableData* row = OrbsPresetTable->FindRow<FOrbsTableData>(data, TEXT("It is Not exist Row"));
		FOrbsDynamicData tmp;
		tmp.OrbsName = row->OrbsName;
		tmp.OrbsColor = row->OrbsColor;
		tmp.OrbsOption = row->OrbsOption;
		tmp.CurrentAmount = 0;
		OrbsList.Add(data);
		OrbsDatas.Add(tmp);
	}
}

void UOrbsManagerComponent::DecreaseOrbs(FName orbNm, int32 val)
{
	for (int i = 0; i < OrbsDatas.Num(); i++)
	{
		if (OrbsDatas[i].OrbsName.IsEqual(orbNm))
		{
			OrbsDatas[i].CurrentAmount = OrbsDatas[i].CurrentAmount - val;
		}
	}
}



void UOrbsManagerComponent::RefreshSpawners()
{
	//orbs를 spawn하기 위한 send 요청이 있는지 확인한다.
	if (CurrentSendOperations.IsValidIndex(0))
	{
		//check to use optimization for queue logic
		if (bEnableSpawnerOptimization)
		{
			//check if active spawners are less than max count
			if (ActiveSendSpawners.Num() < MaxSpawnersLocalCountOptimization)
			{

				TArray<AActor*> tmpSpawnerActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOrbsSpawnerActor::StaticClass(), tmpSpawnerActors);

				//check if active spawners in world are also less than max allowable count
				if (tmpSpawnerActors.Num() >= MaxSpawnersWorldCountOptimization)
				{
					//다시 함수 RefreshSpawners실행
					bRefreshSpawnersExecute = true;
					fRefreshSpawnersTimer = UKismetMathLibrary::RandomFloatInRange(MinSpawnerFrameDelay, MaxSpawnerFrameDelay);
				}

			}
			else
			{
				//다시 함수 RefreshSpawners실행
				bRefreshSpawnersExecute = true;
				fRefreshSpawnersTimer = UKismetMathLibrary::RandomFloatInRange(MinSpawnerFrameDelay, MaxSpawnerFrameDelay);
			}
		}

		LatestSendOperation = CurrentSendOperations[0];

		CurrentSendOperations.RemoveAt(0);

		if (LatestSendOperation.SpawnerClass != nullptr)
		{
			//LatestSpawnerClass = LatestSendOperation.SpawnerClass;
		}
		else
		{
		}

		LatestSpawnerClass = DefaultSpawnerClass;

		//spawn orbs spawner at location null.
		FActorSpawnParameters spawnParam;
		spawnParam.Owner = GetOwner();
		FVector spawnVec = GetOwner()->GetRootComponent()->GetComponentLocation();
		spawnVec.Z += 130.f;
		FTransform transParam = UKismetMathLibrary::MakeTransform(FVector(0, 0, 0), FRotator(0, 0, 0), spawnVec);

		//keep latest reference for use
		LatestSpawnerActor = GetWorld()->SpawnActor<AOrbsSpawnerActor>(LatestSpawnerClass->StaticClass(), transParam, spawnParam);
		
		//attach it to specified source object socket.
		//USceneComponent* SceneComp = Cast<USceneComponent>(SourceMeshObject);
		if (SourceMeshObject)
		{
			LatestSpawnerActor->AttachToComponent(SourceMeshObject, FAttachmentTransformRules::KeepRelativeTransform, SourceMeshSocket);
		}
		else
		{
			LatestSpawnerActor->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, "None");
		}


		ActiveSendSpawners.Add(LatestSpawnerActor);
		ActiveSendOperations.Add(LatestSendOperation);

		LatestSpawnerActor->SpawnOrbs(GetOwner(), this, LatestSendOperation);

		//델리게이트 함수 호출
		EdOrbSendData.Broadcast(LatestSendOperation);

		if (LatestSendOperation.TargetActor)
		{
			if (LatestSendOperation.TargetActor->GetComponentByClass(this->StaticClass()))
			{
				FOrbsReceiveData receviedData;
				receviedData.SenderActor = GetOwner();
				receviedData.SpawnerActor = LatestSpawnerActor;
				receviedData.Type = LatestSendOperation.Type;
				receviedData.Amount = LatestSendOperation.Amount;
				receviedData.Scale = LatestSendOperation.Scale;
				receviedData.SendOption = LatestSendOperation.SendOption;
				EdOrbReceiveData.Broadcast(receviedData);
			}

		}

		RefreshSpawners();
	}

}

// Called every frame
void UOrbsManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//LOG_SCREEN_T("RED ORBS : %d", OrbsDatas[0].CurrentAmount);
}


//오브의 값을 리턴한다.
int32 UOrbsManagerComponent::SetOrbs(FName type, int32 amount, EOrbScale scale, bool bAddValue, bool bCalculateOnly)
{
	int32 LocalTypeIdx;
	int32 LocalValue = amount;
	int32 LocalScale = (int32)scale;
	bool bLocalIsOrbs;
	FOrbsDynamicData LocalOrbsData;
	bool bLocalAdd = bAddValue;
	bool bLocalCalculate = bCalculateOnly;

	GetOrbs(type, bLocalIsOrbs, LocalTypeIdx, LocalOrbsData);

	int32 LocalCurrentAmount = OrbsDatas[LocalTypeIdx].CurrentAmount;

	int32 LocalScaleModifiers = LocalValue * ScaleModifiers[LocalScale];
	int32 ReturnAmount = LocalScaleModifiers + LocalCurrentAmount;

	//Orb의 양(Amount)을 축적 시킬건지, 그냥 값만 넘길건지 정한다.
	//true일 경우 축적한 값 ReturnAmount, false일 경우 해당 orb의 amount값
	int32 LocalTargetAmount = UKismetMathLibrary::SelectInt(ReturnAmount, LocalScaleModifiers, bLocalAdd);

	//LOG_SCREEN("ReturnAmount: %d, LocalValue: %d, scale: %d, LocalTargetAmount:%d", ReturnAmount, LocalValue, (int32)scale, LocalTargetAmount);

	//int32 타입의 정수 범위가 초과 되었을 경우 음수로 가는 것을 막기 위해 clamp 사용
	int32 LocalNewAmount = UKismetMathLibrary::Clamp(LocalTargetAmount, 0, MaxOrbsValue);

	if (!bLocalCalculate)
	{
		OrbsDatas[LocalTypeIdx].CurrentAmount = LocalNewAmount;
	}

	//델리게이트 호출
	EdOrbSetValue.Broadcast(type, amount, scale, bAddValue);
	
	float fRet = UKismetMathLibrary::Abs((LocalNewAmount - LocalCurrentAmount) / ScaleModifiers[(int32)scale]);

	return fRet;
}



void UOrbsManagerComponent::OrbsSpawnWidget()
{
	TArray<UUserWidget*> PlayerHudWidgets;

	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), PlayerHudWidgets, PlayerHud);

	//월드에 추가의 위젯이 있는지 확인한다.
	if (PlayerHudWidgets.Num() > 0)
	{
		Cast<UPlayerWidget>(PlayerHudWidgets[0])->InitOrbsWidget(this);
	}

}

void UOrbsManagerComponent::OrbsSpawnWidgetInStore()
{
	TArray<UUserWidget*> StoreWidgets;

	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), StoreWidgets, StoreHud);

	//월드에 추가의 위젯이 있는지 확인한다.
	if (StoreWidgets.Num() > 0)
	{
		Cast<UStoreWidget>(StoreWidgets[0])->InitOrbsWidget(this);
		gi->SetAmount(OrbsDatas[0].CurrentAmount);
	}
}

//

void UOrbsManagerComponent::OrbsRefreshWidget()
{
	TArray<UUserWidget*> PlayerHudWidgets;

	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), PlayerHudWidgets, PlayerHud);

	//월드에 추가의 위젯이 있는지 확인한다.
	if (PlayerHudWidgets.Num() > 0)
	{
		Cast<UPlayerWidget>(PlayerHudWidgets[0])->UpdateOrbs();
	}
}

void UOrbsManagerComponent::RemoveSpawner(AOrbsSpawnerActor* Spawner)
{
	RemoveSpawnerIndex = ActiveSendSpawners.Find(Spawner);
	if (RemoveSpawnerIndex >= 0)
	{
		ActiveSendSpawners.RemoveAt(RemoveSpawnerIndex);
		ActiveSendOperations.RemoveAt(RemoveSpawnerIndex);
	}
	RefreshSpawners();
}

bool UOrbsManagerComponent::SendOrbsMultiData(AActor* TargetActor, TArray<FOrbsSendData> SendOrbsDatas)
{
	bool bReturnSuccess = false;

	for (auto SendData : SendOrbsDatas)
	{
		if (SendOrbsData(TargetActor, SendData))
		{
			bReturnSuccess = true;
		}
	}

	return bReturnSuccess;
}

bool UOrbsManagerComponent::SendOrbsData(AActor* TargetActor, FOrbsSendData SendData)
{
	bool bReturnSuccess = false;
	int32 idx = 0;
	FOrbsDynamicData tmp;
	SendData.TargetActor = TargetActor;

	GetOrbs(SendData.Type, bReturnSuccess, idx, tmp);

	if (bReturnSuccess)
	{
		if (SendData.ScalePercentage.IsValidIndex(0))
		{
			SendOrbsPercentage(SendData, false);
		}
		else
		{
			SendOrbsAmount(SendData);
		}


	}

	return bReturnSuccess;
}

bool UOrbsManagerComponent::SendOrbsPercentage(FOrbsSendData SendData, bool PercentageAutoMode)
{
	bool LocalSuccess = false;
	TArray<int32> OutPutAmounts;
	TArray<int32> tmp;
	int32 LocalExisitingAmount;
	FOrbsSendData LocalSendData = SendData;

	SendOrbsCalculateAmount(LocalSendData.Type, LocalSendData.Amount, LocalSendData.ScalePercentage, LocalSendData.bInfinitySend, PercentageAutoMode, OutPutAmounts, tmp);

	for (int32 i = OutPutAmounts.Num() - 1; i >= 0; i--)
	{
		if (LocalSendData.bInfinitySend)
		{
			LocalExisitingAmount = OutPutAmounts[i];
		}
		else
		{
			LocalExisitingAmount = SetOrbs(LocalSendData.Type, (OutPutAmounts[i] * (-1)), static_cast<EOrbScale>(i), true, LocalSendData.bDynamicDecrease);
		}

		if (LocalExisitingAmount > 0)
		{
			LocalSendData.Amount = LocalExisitingAmount;
			LocalSendData.Scale = static_cast<EOrbScale>(i);
			SendOrbsSpawn(LocalSendData);
			LocalSuccess = true;
		}
	}

	return LocalSuccess;
}

void UOrbsManagerComponent::SendOrbsSpawn(FOrbsSendData SendData)
{
	CurrentSendOperations.Add(SendData);
	RefreshSpawners();

}

void UOrbsManagerComponent::SetOrbsSourceMeshReference(USceneComponent* mo, FName socNm)
{
	SourceMeshObject = mo;
	SourceMeshSocket = socNm;
}

void UOrbsManagerComponent::ClearActiveSendingSpawner(int32 ActiveIdx, bool bRemoveAll)
{
	if (bRemoveAll)
	{
		for (AOrbsSpawnerActor* spawner : ActiveSendSpawners)
		{
			spawner->DestroySpawner();
		}
	}
	else
	{
		if (ActiveSendSpawners.IsValidIndex(ActiveIdx))
		{
			ActiveSendSpawners[ActiveIdx]->DestroySpawner();
		}
	}
}

void UOrbsManagerComponent::ClearCurrentSendingQueue(int32 ActiveIdx, bool bRemoveAll)
{
	if (bRemoveAll)
	{
		CurrentSendOperations.Empty();
	}
	else
	{
		if (CurrentSendOperations.IsValidIndex(ActiveIdx))
		{
			CurrentSendOperations.Empty(ActiveIdx);
		}
	}
}

bool UOrbsManagerComponent::SendOrbsAmount(FOrbsSendData SendData)
{
	FOrbsSendData LocalSendData = SendData;
	FOrbsDynamicData dData;
	bool bLocalOrbsFlag = false;
	bool bRetFlag = false;
	int32 tmp, LocalExisitingAmount;
	GetOrbs(SendData.Type, bLocalOrbsFlag, tmp, dData);

	if (bLocalOrbsFlag)
	{
		LocalExisitingAmount = SendData.Amount;
	}
	else
	{
		LocalExisitingAmount = SetOrbs(SendData.Type, SendData.Amount * -1.0f, SendData.Scale, true, SendData.bDynamicDecrease); //if dynamic, we do calculate only.
	}

	//if existing amount of specified orbs are exist we process to send.
	if (LocalExisitingAmount > 0)
	{
		SendData.Amount = LocalExisitingAmount;
		SendOrbsSpawn(SendData);
		bRetFlag = true;
	}
	else
	{
		bRetFlag = true;
	}

	return bRetFlag;
}

void UOrbsManagerComponent::SendOrbsCalculateAmount(FName type, int32 Amount,
	TArray<float> PercentageOfScales,
	bool bInfinitySend,
	bool bPercentageAutoMode,
	TArray<int32>& OutPutAmounts,
	TArray<int32>& ScaledAmounts)
{
	//
	bool isOrbs;
	bool bLocalAutoMode;
	int32 idx, LocalAmount, LocalScaleIdx, LocalScaleValue;
	FOrbsDynamicData LocalDynamicData;
	float LocalUserPercentage = 0.0f;

	//Orbs정보
	GetOrbs(type, isOrbs, idx, LocalDynamicData);
	if (isOrbs)
	{
		int32 SelectB = UKismetMathLibrary::Clamp(Amount, 0, LocalDynamicData.CurrentAmount);
		LocalAmount = UKismetMathLibrary::SelectInt(Amount, SelectB, bInfinitySend);
		bLocalAutoMode = bPercentageAutoMode;
		OutPutAmounts.Init(0, PercentageOfScales.Num());
		ScaledAmounts.Init(0, PercentageOfScales.Num());

		for (int32 i = PercentageOfScales.Num() - 1; i >= 0; i--)
		{
			LocalScaleIdx = i;

			LocalScaleValue = ScaleModifiers[i];
			
			float TmpPercentage = UKismetMathLibrary::FClamp(PercentageOfScales[i] + LocalUserPercentage, 0.0f, 100.f);

			float AmountRatio = UKismetMathLibrary::Round(LocalAmount * UKismetMathLibrary::MapRangeClamped(TmpPercentage, 0.0f, 100.f, 0.0f, 1.0f));
			
			OutPutAmounts[LocalScaleIdx] = AmountRatio / LocalScaleValue;

			ScaledAmounts[LocalScaleIdx] = OutPutAmounts[LocalScaleIdx] * LocalScaleValue;

			if (bLocalAutoMode)
			{
				LocalAmount = LocalAmount - ScaledAmounts[LocalScaleIdx];
				LocalUserPercentage = TmpPercentage;
			}
		}

	}
}

void UOrbsManagerComponent::BeSetOrbs(FName type, int32 amount, EOrbScale scale, bool bAddValue)
{
}

void UOrbsManagerComponent::BeSendOrbs(FOrbsSendData sendData)
{

}
void UOrbsManagerComponent::BeReceivedOrbs(FOrbsReceiveData receivedData)
{

}

void UOrbsManagerComponent::BeSendSingleOrbs(FOrbsSendData sendData)
{
}

void UOrbsManagerComponent::BeReceiveSingleOrbs(FOrbsReceiveData ReceiveData)
{
	if (ReceiveData.TargetActor->ActorHasTag("Player"))
	{
		gi->RecoveryHpMana(ReceiveData.Type);
		gi->SetAmount(OrbsDatas[0].CurrentAmount);
	}

}
