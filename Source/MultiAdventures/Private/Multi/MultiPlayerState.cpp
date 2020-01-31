// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "MultiPlayerState.h"

#include "LobbyGameMode.h"

void AMultiPlayerState::ReadyStatus_Implementation(EPlayerReadinessStatus PlayerStatus)
{
	if ((GetWorld()->GetAuthGameMode())->GetNumPlayers() == 1)
	{	
		PlayerReadinessStatus = EPlayerReadinessStatus::NOT_ENOUGH_PLAYERS;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMultiPlayerState::NotEnoughPlayersHandle, 1.5f, false);
	}

	else
	{
		PlayerReadinessStatus = PlayerStatus;
		Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode())->CheckPlayersReadiness();
	}
	
}

void AMultiPlayerState::NotEnoughPlayersHandle()
{
	PlayerReadinessStatus = EPlayerReadinessStatus::NOT_READY;
	GetWorldTimerManager().ClearTimer(TimerHandle);
}
