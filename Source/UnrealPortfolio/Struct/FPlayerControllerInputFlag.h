// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPlayerControllerInputFlag.generated.h"
UCLASS()
class UNREALPORTFOLIO_API AFPlayerControllerInputFlag : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPlayerControllerInputFlag();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};


// * 구조체를 블루프린트에서 사용하기
// Struct는 접미어로 F를 붙여야함
USTRUCT(Atomic, BlueprintType)
//플레이어가 컨트롤러로 입력한 정보를 저장하는 struct
struct FPlayerControllerInputFlag
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isLeftClickDown = false; //왼쪽 클릭 다운 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isRightClickDown = false; //오른쪽 클릭 다운 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isLeftCharge = false; //왼쪽 클릭 차징

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isRightCharge = false; //오른쪽 클릭 차징

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isSPress = false; //S버튼 Pressed 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isWPress = false; //W버튼 Pressed 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isAPress = false; //A버튼 Pressed 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isDPress = false; //D버튼 Pressed 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isFPress = false; //F버튼 Down 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isLeftClickPressed = false; //왼쪽클릭 프레스 이벤트인지

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isRightClickPressed = false; //왼쪽클릭 프레스 이벤트인지

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isShiftPress = false; //쉬프트 프레스인지.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isSpacePress = false; //스페이스바 프레스인지.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isSDown = false; //S버튼 Down 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isWDown = false; //W버튼 Down 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isADown = false; //A버튼 Down 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isDDown = false; //A버튼 Down 여부

};

