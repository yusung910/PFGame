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


// * ����ü�� �������Ʈ���� ����ϱ�
// Struct�� ���̾�� F�� �ٿ�����
USTRUCT(Atomic, BlueprintType)
//�÷��̾ ��Ʈ�ѷ��� �Է��� ������ �����ϴ� struct
struct FPlayerControllerInputFlag
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isLeftClickDown = false; //���� Ŭ�� �ٿ� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isRightClickDown = false; //������ Ŭ�� �ٿ� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isLeftCharge = false; //���� Ŭ�� ��¡

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isRightCharge = false; //������ Ŭ�� ��¡

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isSPress = false; //S��ư Pressed ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isWPress = false; //W��ư Pressed ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isAPress = false; //A��ư Pressed ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isDPress = false; //D��ư Pressed ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isFPress = false; //F��ư Down ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isLeftClickPressed = false; //����Ŭ�� ������ �̺�Ʈ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isRightClickPressed = false; //����Ŭ�� ������ �̺�Ʈ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isShiftPress = false; //����Ʈ ����������.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isSpacePress = false; //�����̽��� ����������.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isSDown = false; //S��ư Down ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isWDown = false; //W��ư Down ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isADown = false; //A��ư Down ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isDDown = false; //A��ư Down ����

};

