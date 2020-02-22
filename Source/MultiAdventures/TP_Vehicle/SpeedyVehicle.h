// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpeedyVehicle.generated.h"

USTRUCT()
struct FVehicleMove
{
	GENERATED_USTRUCT_BODY()

	FVehicleMove()
	{

	}

	FVehicleMove(float InThrottle, float InSteeringThrow, float DeltaT, float InTime) : 
		Throttle(InThrottle), 
		SteeringThrow(InSteeringThrow), 
		DeltaTime(DeltaT), 
		Time(InTime)
	{
		
	}

	UPROPERTY()
		float Throttle;
	UPROPERTY()
		float SteeringThrow;
	UPROPERTY()
		float DeltaTime;
	UPROPERTY()
		float Time;
};

USTRUCT()
struct FVehicleState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		FTransform CarTransform;
	UPROPERTY()
		FVector Velocity;
	UPROPERTY()
		FVehicleMove LastMove;

};


UCLASS()
class MULTIADVENTURES_API ASpeedyVehicle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpeedyVehicle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void SimulateMove(FVehicleMove Move);

	void MoveForward(float Value);
	void MoveRight(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SendMove(FVehicleMove Move); // rename

	UFUNCTION()
		void OnReplicated_ServerState();

	void UpdateLocation(FVehicleMove Move);
	void UpdateRotation(FVehicleMove Move);

	FVector GetAirResistance();
	FVector GetRollingResistance();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/* kg */
	UPROPERTY(EditDefaultsOnly, Category = Properties)
		float Mass = 1000.f;

	/* N */
	UPROPERTY(EditDefaultsOnly, Category = Movement)
		float MaxDrivingForce = 10000.f; 

	/* Degrees Per Second -- (degrees / sec) */
	/* Removed bc of it got replaced by TurningCircleRadius prop.
	UPROPERTY(EditDefaultsOnly, Category = Movement)
		float MaxDPS = 90.f;*/

	/* The higher drag coefficient is the higher air resistance will be */
	UPROPERTY(EditDefaultsOnly, Category = Movement)
		float DragCoefficient = 25.f;

	/* The higher drag coefficient is the higher rolling resistance will be */
	UPROPERTY(EditDefaultsOnly, Category = Movement)
		float RollingResistanceCoefficient = .015f;

	/* Minimum car turning radius (m) */
	UPROPERTY(EditDefaultsOnly, Category = Movement)
		float MinTurningRadius = 10.f;
	
	float Speed = 20.f;
	FVector Velocity;

	UPROPERTY(ReplicatedUsing = OnReplicated_ServerState)
	FVehicleState ServerState;

	/* will be initialized & changed in Move Events */
		float Throttle;
		float SteeringThrow;
	/* ********************************** */

	FHitResult* HitResult;
};
