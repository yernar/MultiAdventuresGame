// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpeedyVehicle.generated.h"

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
	void MoveForward(float Value);
	void MoveRight(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_MoveForward(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_MoveRight(float Value);

	UFUNCTION()
		void OnReplicated_CarTransform();

	void UpdateLocation(float DeltaTime);
	void UpdateRotation(float DeltaTime);

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

	UPROPERTY(Replicated)
		FVector Velocity;
	
	float Speed = 20.f;

	/* will be initialized & changed in Move Events */
	UPROPERTY(Replicated)
		float Throttle;
	UPROPERTY(Replicated)
		float SteeringThrow;
	/* ********************************** */

	UPROPERTY(ReplicatedUsing = OnReplicated_CarTransform)
		FTransform CarTransform;

	FHitResult* HitResult;
};
