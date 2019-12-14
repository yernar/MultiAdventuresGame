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

	if (NumPlayers == 3)
		GetWorld()->ServerTravel("/Game/Maps/MainLevel?listen");
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumPlayers;
}
