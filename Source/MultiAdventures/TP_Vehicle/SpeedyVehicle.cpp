// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "SpeedyVehicle.h"

#include "UnrealNetwork.h"

// Sets default values
ASpeedyVehicle::ASpeedyVehicle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ASpeedyVehicle::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
		NetUpdateFrequency = 1.f;
}

void ASpeedyVehicle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASpeedyVehicle, ServerState)
}

void ASpeedyVehicle::SimulateMove(const FVehicleMove& Move)
{
	UpdateLocation(Move);
	UpdateRotation(Move);
}

void ASpeedyVehicle::MoveForward(float Value)
{
	Throttle = Value;
}

void ASpeedyVehicle::MoveRight(float Value)
{
	SteeringThrow = Value;
}

bool ASpeedyVehicle::Server_SendMove_Validate(FVehicleMove Move)
{
	return true; // TODO: check if clients are not cheating!
}

void ASpeedyVehicle::Server_SendMove_Implementation(FVehicleMove Move)
{
	SimulateMove(Move);

	ServerState.CarTransform = GetActorTransform();
	ServerState.Velocity = Velocity;
	ServerState.LastMove = Move;
}

//bool ASpeedyVehicle::Server_MoveRight_Validate(float Value)
//{
//	return (Value <= 1.f ? true : (Value >= 1.f ? true : false));
//}
//
//void ASpeedyVehicle::Server_MoveRight_Implementation(float Value)
//{
//	SteeringThrow = Value;
//}

void ASpeedyVehicle::OnReplicated_ServerState()
{
	SetActorTransform(ServerState.CarTransform);
	Velocity = ServerState.Velocity;

	ClearAcknowledgedMoves(ServerState.LastMove);

	for (const FVehicleMove& Move : UnaknowledgedMoves)
	{
		SimulateMove(Move);
	}
}

void ASpeedyVehicle::ClearAcknowledgedMoves(FVehicleMove& LastMove)
{
	TArray<FVehicleMove> NewMoves;

	for (const FVehicleMove& Move : UnaknowledgedMoves)
		if (Move.Time > LastMove.Time)
			NewMoves.Add(Move);

	UnaknowledgedMoves = NewMoves;
}

void ASpeedyVehicle::UpdateLocation(FVehicleMove Move)
{
	FVector Force = GetActorForwardVector() * MaxDrivingForce * Move.Throttle + GetAirResistance() + GetRollingResistance();
	FVector Acceleration = Force / Mass;
	Velocity += Acceleration * Move.DeltaTime;

	HitResult = new FHitResult();
	AddActorWorldOffset(Velocity * 100 * Move.DeltaTime, true, HitResult); // converting velocity to cm., as initially it was m.
	
	if (HitResult->IsValidBlockingHit())
		Velocity = FVector::ZeroVector;

	delete HitResult;
}

void ASpeedyVehicle::UpdateRotation(FVehicleMove Move)
{
	float DeltaLocation = FVector::DotProduct(GetActorForwardVector(), Velocity) * Move.DeltaTime;
	float RotationAngleRadians =  DeltaLocation / MinTurningRadius * Move.SteeringThrow;
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
	
	if (IsLocallyControlled())
	{
		FVehicleMove Move(Throttle, SteeringThrow, DeltaTime, GetWorld()->TimeSeconds); // TODO: set normal time
		Server_SendMove(Move);		

		if (!HasAuthority())
		{
			UnaknowledgedMoves.Add(Move);
			SimulateMove(Move);
		}

		FVehicleMove LastMove;

		ClearAcknowledgedMoves(LastMove);

	}	

	if (GetRemoteRole() == ROLE_AutonomousProxy)
	{
		FVehicleMove Move(Throttle, SteeringThrow, DeltaTime, GetWorld()->TimeSeconds); // TODO: set normal time
		SimulateMove(Move);

		UnaknowledgedMoves.Add(Move);
		Server_SendMove(Move);
	}

	// if we are the server and controlling the pawn
	if (GetRemoteRole() == ROLE_Authority && GetRemoteRole() == ENetRole::ROLE_SimulatedProxy)
	{
		FVehicleMove Move(Throttle, SteeringThrow, DeltaTime, GetWorld()->TimeSeconds); // TODO: set normal time
		Server_SendMove(Move);
	}

	if (ENetRole::ROLE_SimulatedProxy == GetRemoteRole())
		SimulateMove(ServerState.LastMove);
}

// Called to bind functionality to input
void ASpeedyVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASpeedyVehicle::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASpeedyVehicle::MoveRight);
}

