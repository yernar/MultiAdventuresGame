// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "LobbyGameMode.h"
#include "MultiPlayerState.h"
#include "LobbyHUD.h"
#include "MainMenu.h"
#include "MultiGameInstance.h"

#include "GameFramework\GameStateBase.h"

ALobbyGameMode::ALobbyGameMode()
{
	bUseSeamlessTravel = true;
	PrimaryActorTick.bCanEverTick = true;

	PlayerStateClass = AMultiPlayerState::StaticClass();
	HUDClass = ALobbyHUD::StaticClass();
	GameMapLocation = new FString(DefaultMapsDirectory);	
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetGameMap();
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
	GetWorld()->ServerTravel(*GameMapLocation);
}

void ALobbyGameMode::SetGameMap()
{
	FString MapName;

	switch (Cast<UMultiGameInstance>(GetGameInstance())->GetMainMenuWidget()->GetSelectedGameMode())
	{
	case PJ:
		MapName = "PlatformJumper";
		break;
	case SV:
		MapName = "SpeedyVehicle";		
		break;
	}

	*GameMapLocation += MapName;
}
