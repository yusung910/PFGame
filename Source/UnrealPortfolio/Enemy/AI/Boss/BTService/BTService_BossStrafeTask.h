// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "EBossStatus.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_BossStrafeTask.generated.h"

class UEnvQuery;

UCLASS()
class UNREALPORTFOLIO_API UBTService_BossStrafeTask : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_BossStrafeTask();

	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData);

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	void DoStrafeQuery();
	void MoveToQueryResult(TSharedPtr<FEnvQueryResult> result);

private:
	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
		class UEnvQuery* LeftEQS;
	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
		class UEnvQuery* RightEQS;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
		float SearchRadius = 1000.f; //·¥´ý ÁöÁ¡ Å½»ö ¹üÀ§
	UPROPERTY()
		class ABossCharacter* Boss;

	UPROPERTY()
		class ABossAIController* BossController;


};
