// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "MainPlayerState.h"

#include "LobbyGameMode.h"

void AMainPlayerState::ReadyStatus_Implementation(EPlayerReadinessStatus PlayerStatus)
{
	PlayerReadinessStatus = ((GetWorld()->GetAuthGameMode())->GetNumPlayers() == 1 ? 
		EPlayerReadinessStatus::NOT_ENOUGH_PLAYERS : PlayerStatus);

	Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode())->CheckPlayersReadiness();
}