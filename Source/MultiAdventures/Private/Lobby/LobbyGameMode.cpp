// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "LobbyGameMode.h"
#include "MultiPlayerState.h"
#include "LobbyHUD.h"

#include "GameFramework\GameStateBase.h"

ALobbyGameMode::ALobbyGameMode()
{
	bUseSeamlessTravel = true;

	PlayerStateClass = AMultiPlayerState::StaticClass();
	HUDClass = ALobbyHUD::StaticClass();
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);	
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void ALobbyGameMode::CheckPlayersReadiness()
{
	for (const auto* PlayerState : Cast<AGameStateBase>(GetWorld()->GetGameState())->PlayerArray)
	{
		EPlayerReadinessStatus PlayersStatus = Cast<AMultiPlayerState>(PlayerState)->PlayerReadinessStatus;
		if (PlayersStatus != EPlayerReadinessStatus::READY)
			return;
	}
	TravelGameMap();	
}

void ALobbyGameMode::TravelGameMap()
{
	GetWorld()->ServerTravel("/Game/Maps/MainLevel?listen");
}
