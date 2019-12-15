// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "LobbyGameMode.h"

ALobbyGameMode::ALobbyGameMode()
{
	bUseSeamlessTravel = true;
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumPlayers;

	if (NumPlayers >= 2)	
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ALobbyGameMode::TravelGameMap, 10.f, false);
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumPlayers;
}

void ALobbyGameMode::TravelGameMap()
{
	GetWorld()->ServerTravel("/Game/Maps/MainLevel?listen");
}
