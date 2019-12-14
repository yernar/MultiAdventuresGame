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
