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
	Velocity = GetActorForwardVector() * Speed * Value;
}

// Called every frame
void ASpeedyVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldOffset(Velocity * 100 * DeltaTime);
}

// Called to bind functionality to input
void ASpeedyVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASpeedyVehicle::MoveForward);

}

