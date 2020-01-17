// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TP_VehicleGameMode.h"
#include "TP_VehicleHud.h"

#include "UObject/ConstructorHelpers.h"

ATP_VehicleGameMode::ATP_VehicleGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> VehiclePawnClass(TEXT("/Game/Pawns/Vehicle/Blueprints/BP_SpeedyVehicle"));
	if (VehiclePawnClass.Class)
		DefaultPawnClass = VehiclePawnClass.Class;

	HUDClass = ATP_VehicleHud::StaticClass();
}
