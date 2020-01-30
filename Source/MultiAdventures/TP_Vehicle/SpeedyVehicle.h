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

	void UpdateLocation(float DeltaTime);
	void UpdateRotation(float DeltaTime);

	FVector GetAirResistance();

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
	UPROPERTY(EditDefaultsOnly, Category = Movement)
		float MaxDPS = 90.f;

	/* The higher drag coefficient is the higher air resistance will be */
	UPROPERTY(EditDefaultsOnly, Category = Movement)
		float DragCoefficient = 25.f;

	FVector Velocity;
	float Speed = 20.f;

	/* will be initialized in Move Events */
	float Throttle;
	float SteeringThrow;
	/* ********************************** */

	FHitResult* HitResult;
};
