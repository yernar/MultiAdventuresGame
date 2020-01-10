// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#include "MultiGameMode.h"
#include "MultiTPCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultiGameMode::AMultiGameMode()
{
	// set default pawn class to our Blueprinted character Blueprint
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Character/ThirdPerson/Blueprints/BP_MultiTPCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
