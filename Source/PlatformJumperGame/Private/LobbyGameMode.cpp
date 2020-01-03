// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "LobbyGameMode.h"
#include "MainPlayerState.h"

#include "GameFramework\GameStateBase.h"

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

void ALobbyGameMode::CheckPlayersReadiness()
{
	for (const auto* PS : Cast<AGameStateBase>(GetWorld()->GetGameState())->PlayerArray)
	{
		EPlayerReadinessStatus PlayersStatus = Cast<AMainPlayerState>(PS)->PlayerReadinessStatus;
		if (PlayersStatus != EPlayerReadinessStatus::READY)
			return;
	}
	TravelGameMap();
	// GetWorldTimerManager().SetTimer(TimerHandle, this, &ALobbyGameMode::TravelGameMap, 10.f, false);
}

void ALobbyGameMode::TravelGameMap()
{
	GetWorld()->ServerTravel("/Game/Maps/MainLevel?listen");
}
