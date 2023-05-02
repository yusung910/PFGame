// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OrbStructActor.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "OrbsManagerComponent.generated.h"
UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FDynamicSignature_SetOrbs, FName, Type, int32, Amount, EOrbScale, scale, bool, bAddValue);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDynamicSignature_SendedOrbs, FOrbsSendData, sendData);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDynamicSignature_ReceivedOrbs, FOrbsReceiveData, receiveData);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDynamicSignature_SendOrbSingle, FOrbsSendData, sendData);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDynamicSignature_ReceiveOrbSingle, FOrbsReceiveData, receiveData);
//


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALPORTFOLIO_API UOrbsManagerComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	// Sets default values for this component's properties
	UOrbsManagerComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public: //Delegate 함수
	UFUNCTION()
		void BeSetOrbs(FName type, int32 amount, EOrbScale scale, bool bAddValue);

	UFUNCTION()
		void BeSendOrbs(FOrbsSendData SendData);

	UFUNCTION()
		void BeReceivedOrbs(FOrbsReceiveData receivedData);

	UFUNCTION()
		void BeSendSingleOrbs(FOrbsSendData SendData);

	UFUNCTION()
		void BeReceiveSingleOrbs(FOrbsReceiveData ReceiveData);

public:
	UFUNCTION()
		void RefreshSpawners();

	UFUNCTION()
		void LoadOrbs();

	UFUNCTION()
		void DecreaseOrbs(FName orbNm, int32 val);

	UFUNCTION()
		int32 SetOrbs(FName type, int32 amount, EOrbScale scale, bool bAddValue, bool bCalculateOnly);

	UFUNCTION()
		void GetOrbs(FName type, bool& retVal, int32& foundIdx, FOrbsDynamicData& orbsData);

	UFUNCTION()
		void OrbsSpawnWidget();


	UFUNCTION()
		void OrbsSpawnWidgetInStore();

	UFUNCTION()
		void OrbsRefreshWidget();

	UFUNCTION()
		void RemoveSpawner(AOrbsSpawnerActor* Spawner);



	//Orbs를 전달 하기 위한 함수
	UFUNCTION()
		bool SendOrbsMultiData(AActor* TargetActor, TArray<FOrbsSendData> SendOrbsDatas);

	UFUNCTION()
		bool SendOrbsData(AActor* TargetActor, FOrbsSendData SendData);

	UFUNCTION()
		bool SendOrbsPercentage(FOrbsSendData SendData, bool PercentageAutoMode);

	UFUNCTION()
		bool SendOrbsAmount(FOrbsSendData SendData);

	UFUNCTION()
		void SendOrbsCalculateAmount(FName type, int32 Amount,
			TArray<float> PercentageOfScales,
			bool bInfinitySend, bool bPercentageAutoMode,
			TArray<int32>& OutPutAmounts,
			TArray<int32>& ScaledAmounts);
	UFUNCTION()
		void SendOrbsSpawn(FOrbsSendData SendData);

	UFUNCTION()
		void SetOrbsSourceMeshReference(USceneComponent* mo, FName socNm);

	UFUNCTION()
		void ClearActiveSendingSpawner(int32 ActiveIdx, bool bRemoveAll);

	UFUNCTION()
		void ClearCurrentSendingQueue(int32 ActiveIdx, bool bRemoveAll);
public:
	FDynamicSignature_SetOrbs EdOrbSetValue;
	FDynamicSignature_SendedOrbs EdOrbSendData;
	FDynamicSignature_ReceivedOrbs EdOrbReceiveData;
	FDynamicSignature_SendOrbSingle EdOrbSendSingle;
	FDynamicSignature_ReceiveOrbSingle EdOrbReceiveSingle;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE TArray<FOrbsDynamicData> GetOrbsDatas() { return OrbsDatas; }
	FORCEINLINE void SetRedData(int32 n) { OrbsDatas[0].CurrentAmount = n; }

	FORCEINLINE AOrbsProjectilesBaseActor* GetDefaultProjectilesClass() { return DefaultProjectilesClass; }

	FORCEINLINE AOrbsSpawnerActor* GetDefaultSpawnerClass() { return DefaultSpawnerClass; }
	FORCEINLINE USceneComponent* GetSourceMeshObject() { return SourceMeshObject; }
	FORCEINLINE FName GetSourceMeshSocket() { return SourceMeshSocket; }
	FORCEINLINE bool GetEnableOrbsPull() { return EnableOrbsPull; }

private:
	UPROPERTY()
		class UMainGameInstance* gi;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Orbs Settings")
		class UDataTable* OrbsPresetTable;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Orbs Settings")
		TArray<int32> initialValues = { 1000, 0, 0 };

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Orbs Settings")
		TArray<int32> ScaleModifiers = { 1, 3, 5, 7, 10 };

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Orbs Settings")
		int32 MaxOrbsValue = 999999999;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Orbs Misc")
		bool EnablePlayerWidget = true;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Orbs Misc")
		class AOrbsSpawnerActor* DefaultSpawnerClass;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Orbs Misc")
		class AOrbsProjectilesBaseActor* DefaultProjectilesClass;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Orbs Misc")
		bool EnableOrbsPull = false;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		TArray<FName> OrbsList;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		TArray<FOrbsDynamicData> OrbsDatas;

	//Convert source mesh object reference to any scene component. (Skeletal mesh, static mesh, or scene component)
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		class USceneComponent* SourceMeshObject;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		FName SourceMeshSocket; //attach it to specified source object socket.

	UPROPERTY()
		TSubclassOf<class UUserWidget> PlayerHud; //메인 화면 위젯


	UPROPERTY()
		TSubclassOf<class UUserWidget> StoreHud; //상점 화면 위젯

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Spawners Queue Private Data")
		TArray<FOrbsSendData> CurrentSendOperations;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Spawners Queue Private Data")
		TArray<AOrbsSpawnerActor*> ActiveSendSpawners;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Spawners Queue Private Data")
		FOrbsSendData LatestSendOperation;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Spawners Queue Private Data")
		class AOrbsSpawnerActor* LatestSpawnerClass;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Spawners Queue Private Data")
		class AOrbsSpawnerActor* LatestSpawnerActor;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Spawners Queue Private Data")
		TArray<FOrbsSendData> ActiveSendOperations;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Spawners Queue Private Data")
		int32 RemoveSpawnerIndex = 0;
private:

	//Enable orbs spawner queue optimization, if enabled send operations will go to queue if they pass max limit, so newer operations will wait until older one finish.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Orbs Optimization")
		bool bEnableSpawnerOptimization = true;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Orbs Optimization")
		int32 MaxSpawnersLocalCountOptimization = 4;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Orbs Optimization")
		int32 MaxSpawnersWorldCountOptimization = 10;

	//Quick delays for queue checks after each send operation is started or waiting in queue, randomized between min and max values.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Orbs Optimization")
		float MaxSpawnerFrameDelay = 28.0f;

	//Quick delays for queue checks after each send operation is started or waiting in queue, randomized between min and max values.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Orbs Optimization")
		float MinSpawnerFrameDelay = 14.0f;

	UPROPERTY()
		bool bRefreshSpawnersExecute = false;

	UPROPERTY()
		float fRefreshSpawnersTimer = 0.0f;

	UPROPERTY()
		float fTickCounter = 0.f;
};