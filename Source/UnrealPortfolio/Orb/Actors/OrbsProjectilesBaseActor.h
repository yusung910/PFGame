// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "OrbStructActor.h"
#include "OrbsProjectilesBaseActor.generated.h"



UCLASS()
class UNREALPORTFOLIO_API AOrbsProjectilesBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOrbsProjectilesBaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void InitOrbsData(FOrbsReceiveData ReceiveData);

	UFUNCTION()
		void SetOrbInit();
	UFUNCTION()
		void SetOrbStart();

	UFUNCTION()
		void TickTargetLocation();
	UFUNCTION()
		void TickVelocity();
	UFUNCTION()
		void TickFinish();
	UFUNCTION()
		void TickAlpha();

	UFUNCTION()
		void FinishReceive();
	UFUNCTION()
		void FinishOrbs();

protected:
	UPROPERTY(VisibleAnywhere)
		class USphereComponent* CollisionComponent;
	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(VisibleAnywhere)
		class UParticleSystemComponent* OrbsParticleSystem;
	UPROPERTY(VisibleAnywhere)
		class UParticleSystem* OrbsParticleHit;
	UPROPERTY(VisibleAnywhere)
		class UOrbsManagerComponent* OrbsComponent;

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Extra Data")
		float MaxFrameStartDelay = 16.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Extra Data")
		float MinFrameStartDelay = 8.0f;

	//Enable pull mode, so target character should have active "pull mode" to drag this orbs and collect them.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Extra Data")
		bool bEnableTargetPullMode = false;

	//Radius of target pull mode, to drag orbs to be collected.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Extra Data")
		float TargetPullRadius = 1024.f;

	//check for radius only, if radius mode enabled.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Extra Data")
		bool bEnableTargetPullRadius = false;

	//Always magnet the orbs to target when they are close, this will ignore current detect state, so if orbs close they will be collected.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Extra Data")
		bool bEnableTargetPullMagnet = false;

	//Max lifespan to destroy orb while having valid target, this will autodestroy orb when it was not be able to reach target after seconds.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Extra Data")
		float MaxLifespanWithTarget = 42.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Extra Data")
		float MaxLifespanNoTarget = 7.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Extra Data")
		FVector2D InitialPitchRange = FVector2D(10.0f, 90.0f);

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Extra Data")
		FVector2D InitialYawRange = FVector2D(-180.0f, 180.0f);

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		FOrbsReceiveData InitialData;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		FName Type = "None";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		EOrbScale Scale;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		class AActor* SenderActor;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		class AActor* SpawnerActor;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		class AActor* TargetActor;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		class UOrbsManagerComponent* TargetComp;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		FString SendOptions;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		float DetectDelay = 0.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		float InitialSpeed;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		float TargetSpeed;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		float DetectInterp;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		float TargetInterp;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		bool IsDetect = false;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		bool IsStarted = false;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		bool IsFinished = false;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		FVector TargetLocation;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		FVector TargetVelocity;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		float TargetAlpha = 0.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		FLinearColor OrbsColor;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		FString OrbOptions;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		FVector InitialVelocity;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		FVector StartingVelocity;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Basic Private Data")
		bool bExecuteInitOrbsDataFunc = false;
	FTimerHandle InitOrbsDataHandle;

	FTimerHandle WaitHandle;

	FTimerHandle IsDetectedHandle;

	FTimerHandle FinishTimerHandle;

	FTimerHandle DestroyTimerHandle;
private:
	//Initial speed when orb gets activated and starts detecting target, randomized between min and max values.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MaxInitialSpeed = 700.0f; 

	//Initial speed when orb gets activated and starts detecting target, randomized between min and max values.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MinInitialSpeed = 200.0f;

	//Active speed when orb found target and starts moving toward target, randomized between min and max values.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MaxTargetSpeed = 700.0f;

	//Active speed when orb found target and starts moving toward target, randomized between min and max values.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MinTargetSpeed = 420.0f;

	//Quick delay before orb does detect the target and starts to moving to its direction, randomized between min and max value.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MaxDetectTargetSpeed = 0.7f;

	//Quick delay before orb does detect the target and starts to moving to its direction, randomized between min and max value.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MinDetectTargetSpeed = 0.3f;

	//Interpolation speed of how fast orb smoothly will detect target direction after "detect delay" is finished, randomized between min and max value.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MaxDetectTargetInterpSpeed = 3.5f;

	//Interpolation speed of how fast orb smoothly will detect target direction after "detect delay" is finished, randomized between min and max value.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MinDetectTargetInterpSpeed = 1.4f;

	//Interpolation speed between current speed and target speed, randomized between min and max values.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MaxVelocityTargetInterpSpeed = 6.0f;

	//Interpolation speed between current speed and target speed, randomized between min and max values.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MinVelocityTargetInterpSpeed = 4.2f;

	//Max time for target speed over time power, in seconds, how much time will be passed from usage of min and max target speed powers.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MinTargetTime = 4.0f;

	//Max time for target speed over time power, in seconds, how much time will be passed from usage of min and max target speed powers.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MaxTargetTime = 14.0f;

	//Target speed by time power, starting multipler of speed when time is at "min target time"
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MinTargetSpeedOverTimePower = 1.0f;
	//Target speed by time power, ending multiplier of speed when time is closer to "max target time"
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MaxTargetSpeedOverTimePower = 1.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MinTargetMovingSpeedPower = 2.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MaxTargetMovingSpeedPower = 10.0f;


	//Min time for chaotic over time values, in seconds, how much time will be passed from usage of min and max chaotic values.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MinChaoticTime = 1.0f;

	//Max time for chaotic over time values, in seconds, how much time will be passed from usage of min and max chaotic values.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MaxChaoticTime = 14.0f;

	//Chaotic speed over time, starting speed when chaotic time is at "min chaotic time"
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MinChaoticSpeedOverTime = 0.0f;

	//Chaotic speed over time, ending speed when chaotic time is at "max chaotic time"
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MaxChaoticSpeedOverTime = 420.0f;

	//Chaotic power over time, starting power of chaotic rotations, when chaotic time is at "min chaotic time"
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MinChaoticPowerOverTime = 1.0f;

	//Chaotic power over time, ending power of chaotic rotations, when chaotic time is at "min chaotic time"
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float MaxChaoticPowerOverTime = 1.0f;

	//Enable to rotate by custom interpolation towards target, otherwise it will rotate by default projectile component's "rotation follows velocity"
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		bool bEnableChaoticWithoutTarget = false;

	//Starting initial velocity, orb speed that works before orb gets activated and detects target.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Settings Speed")
		float StartInitialVelocity = 210.0f;

private:
	//Enable to rotate by custom interpolation towards target, otherwise it will rotate by default projectile component's "rotation follows velocity"
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Misc Data")
		bool bRotateByManualInterp = false;

	//Rotate by manual interp speed, if bool is enabled, how fast projectile will rotate toward target.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Misc Data")
		float RotateManualInterpSpeed = 14.0f;

	//Min distance to finish orb delivery, orb needs to be at distance equal to this or closer to finish delivery and selfdestroy.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Misc Data")
		float MinDistanceToFinishDelivery = 42.f;

	//Min distance to quickly interpolate to target, orb needs to be at this distance or closer, to quickly magnet to target.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Misc Data")
		float MinDistanceToMagnetDelivery = 1000000.f;

	//Interpolation speed for min distance interp, this will increase speed of magneting to target when orb is too close to target.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Misc Data")
		float MinDistanceToMagnetInterpSpeed = 10.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Misc Data")
		float LocalVlerpRate = 0.0f;
	//Very simple optimization logic of attached hit particles when orb finish its delivery to target, can be not accurate.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Misc Data")
		int32 HitParticlesMaxAttachOptimization = 7;

	//Scaling factor array, to visually scale orbs particle. The order goes in the scale enum order. By default index 0 is 1x, while index 4 is 10x scale.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Misc Data")
		TArray<float> ScaleList = {1.4f, 1.7f, 2.1f, 3.5f, 4.9f, 5.5f};

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "Editable Misc Data")
		float ScaleMultiplyFactor = 0.77f;
};
