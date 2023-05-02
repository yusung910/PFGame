// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "CoreMinimal.h"
#include "EngineMinimal.h"
#include "DrawDebugHelpers.h"

DECLARE_LOG_CATEGORY_EXTERN(DevLog, Log, All);

// �� �Լ��� ȣ��� �Լ��� �̸��� ������ ��ũ�η� Stringȭ ��Ŵ - ��ó : �̵���� �𸮾� C++ ���� ������ ����
#define LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

// ���� CALLINFO�� �Բ� �ƹ� ������ ���� �α׸� ��� ��ũ�� - ��ó : �̵���� �𸮾� C++ ���� ������ ����
#define LOG_S(Verbosity) UE_LOG(DevLog, Verbosity, TEXT("%s"), *LOG_CALLINFO)

// CALLINFO�� �Բ� �ؽ�Ʈ�� �α׸� ��� ��ũ��
#define LOG(Verbosity, Format, ...) UE_LOG(DevLog, Verbosity, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))

// Screen���ٰ� LOG�� ���� ��� ��ũ��
#define	LOG_SCREEN(Format, ...) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(Format), ##__VA_ARGS__))

// ���� �Լ��� �ð��� �ִ� ��ũ��. Tick���� DeltaTime�� ���ڷ� ������ ���� ����.
#define	LOG_SCREEN_T(Format, ...) GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::Red, FString::Printf(TEXT(Format), ##__VA_ARGS__))

// LOG�� Verbosity�� ���ڿ��� ���� ��ũ��ȭ ��Ų��. 
#define LOG_WARNING(Format, ...) UE_LOG(DevLog, Warning, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))
#define LOG_ERROR(Format, ...) UE_LOG(DevLog, Error, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))

// assert()�� ���� ����� �뵵�� ��밡���� ��ũ��. ...���� retval�� ��.
#define CHECK(Expr, ...) {if(!(Expr)) {LOG(Error, "ASSERTION : %s", "'"#Expr"'"); return __VA_ARGS__;}}

// nullüũ
#define IS_NULL(p) { if(p == nullptr) {LOG(Error, "Object is Null : %s", "'"#p"'"); return; } }

/*    Object Asset�� �޽��ϴ�.
*    ������ Ÿ�ӿ��� �۵��մϴ�.
*    RETURN_POINTER : ���� ������
*    CLASS_TYPE : ���� ������Ʈ�� Ŭ����
*    PATH : ��� (TEXT�� ������ �ʽ��ϴ�.)*/
#define GetObjectAsset(RETURN_POINTER, CLASS_TYPE, PATH)\
static ConstructorHelpers::FObjectFinder<CLASS_TYPE> __##RETURN_POINTER(TEXT(PATH));\
if (__##RETURN_POINTER.Succeeded()) {\
    RETURN_POINTER = __##RETURN_POINTER.Object;\
}\
else RETURN_POINTER = nullptr;\
CHECK(RETURN_POINTER)


/*    Class Asset�� �޽��ϴ�.
*    ������ Ÿ�ӿ��� �۵��մϴ�.
*    RETURN_POINTER : ���� ������
*    CLASS_TYPE : ���� Ŭ���� Ÿ��
*    PATH : ��� (TEXT�� ������ �ʽ��ϴ�.)*/
#define GetClassAsset(RETURN_POINTER, CLASS_TYPE, PATH)\
static ConstructorHelpers::FClassFinder<CLASS_TYPE> __##RETURN_POINTER(TEXT(PATH));\
if (__##RETURN_POINTER.Succeeded()) {\
    RETURN_POINTER = __##RETURN_POINTER.Class;\
}\
else RETURN_POINTER = nullptr;\
CHECK(RETURN_POINTER)

/*
* ��Ƽ������ ȣ���Ѵ�.
*/
#define GetMaterialInst(RET_PTR, PATH)\
UMaterialInstanceConstant* mat = Cast<UMaterialInstanceConstant>(StaticLoadObject(UMaterialInstanceConstant::StaticClass(), NULL, TEXT(PATH)));\
RET_PTR = UMaterialInstanceDynamic::Create(mat, this);\
CHECK(mat)

/*
* ���̾ư��� ������Ʈ�� ȣ���Ѵ�
* RET_PTR : ���� ������
* ATTACH_PTR : ���̾ư��� ������Ʈ�� ��ӽ�ų ���� ������(getMesh());
* COMP_NM : ������Ʈ ��
* PATH: ���̾ư��� ������Ʈ ���
*/
#define GetNiagaraComp(RET_PTR, SYS_PTR, ATTACH_PTR, COMP_NM, PATH)\
RET_PTR = CreateDefaultSubobject<UNiagaraComponent>(TEXT(COMP_NM));\
GetObjectAsset(SYS_PTR, UNiagaraSystem, PATH);\
RET_PTR->SetAsset(SYS_PTR);\
RET_PTR->SetupAttachment(ATTACH_PTR);\
RET_PTR->SetVisibility(false);\
CHECK(SYS_PTR)

//�Ϲ� �Լ����� ����ϴ� �� ���Ͱ��� ���� �׸��� �Լ�
//Tick�Լ������� ������� ���ƾ��Ѵ�.
#define DrawDebugingLine(START, END, COLOR) DrawDebugLine(GetWorld(), START, END, COLOR, true, -1.0F, 0U, 5.F);

//ƽ�Լ������� ����� �� �ִ�.
#define DrawDebugingLineT(START, END) DrawDebugLine(GetWorld(), START, END, FColor::Yellow, false, -1.0F, 0U, 5.F);

#define AddFVectorX(V, f) FVector(V.X + f, V.Y, V.Z);

#define ExceptZFVector(V) FVector(V.X, V.Y, 0.f);