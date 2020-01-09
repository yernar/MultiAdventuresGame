// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "MainPlayerState.h"

#include "LobbyGameMode.h"

void AMainPlayerState::ReadyStatus_Implementation(EPlayerReadinessStatus PlayerStatus)
{
	if ((GetWorld()->GetAuthGameMode())->GetNumPlayers() == 1)
	{	
		PlayerReadinessStatus = EPlayerReadinessStatus::NOT_ENOUGH_PLAYERS;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMainPlayerState::NotEnoughPlayersHandle, 1.5f, false);
	}

	else 
	{
		PlayerReadinessStatus = PlayerStatus;
		Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode())->CheckPlayersReadiness();
	}
	
}

void AMainPlayerState::NotEnoughPlayersHandle()
{
	PlayerReadinessStatus = EPlayerReadinessStatus::NOT_READY;
	GetWorldTimerManager().ClearTimer(TimerHandle);
}
