// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#include "MultiAdventuresGameMode.h"
#include "MultiAdventuresCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultiAdventuresGameMode::AMultiAdventuresGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
