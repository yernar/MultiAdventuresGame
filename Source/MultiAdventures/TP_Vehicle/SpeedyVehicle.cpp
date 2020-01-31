// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "SpeedyVehicle.h"

// Sets default values
ASpeedyVehicle::ASpeedyVehicle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpeedyVehicle::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpeedyVehicle::MoveForward(float Value)
{
	Throttle = Value;
}

void ASpeedyVehicle::MoveRight(float Value)
{
	SteeringThrow = Value;
}

void ASpeedyVehicle::UpdateLocation(float DeltaTime)
{
	FVector Force = GetActorForwardVector() * MaxDrivingForce * Throttle + GetAirResistance() + GetRollingResistance();
	FVector Acceleration = Force / Mass;
	Velocity += Acceleration * DeltaTime;

	HitResult = new FHitResult();
	AddActorWorldOffset(Velocity * 100 * DeltaTime, true, HitResult); // converting velocity to cm., as initially it was m.
	
	if (HitResult->IsValidBlockingHit())
		Velocity = FVector::ZeroVector;

	delete HitResult;
}

void ASpeedyVehicle::UpdateRotation(float DeltaTime)
{
	float DeltaLocation = FVector::DotProduct(GetActorForwardVector(), Velocity) * DeltaTime;
	float RotationAngleRadians =  DeltaLocation / MinTurningRadius * SteeringThrow;
	FQuat RotationDelta(GetActorUpVector(), RotationAngleRadians);

	Velocity = RotationDelta.RotateVector(Velocity);
	AddActorWorldRotation(RotationDelta);
}

FVector ASpeedyVehicle::GetAirResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() *  DragCoefficient; // acts against car moving direction
}

FVector ASpeedyVehicle::GetRollingResistance()
{
	float AccelerationDueToGravity = -GetWorld()->GetGravityZ() / 100; // converting m. to cm.
	float NormalForce = AccelerationDueToGravity * Mass;
	return -Velocity.GetSafeNormal() * NormalForce * RollingResistanceCoefficient;
}

// Called every frame
void ASpeedyVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateLocation(DeltaTime);	
	UpdateRotation(DeltaTime);	
}

// Called to bind functionality to input
void ASpeedyVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASpeedyVehicle::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASpeedyVehicle::MoveRight);
}

