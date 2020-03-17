// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#include "MultiGameMode.h"

#include "UObject/ConstructorHelpers.h"

AMultiGameMode::AMultiGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Pawns/ThirdPerson/Blueprints/BP_MultiTPCharacter"));
	if (PlayerPawnBPClass.Class)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
