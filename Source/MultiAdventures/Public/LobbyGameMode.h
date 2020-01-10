// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MultiGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIADVENTURES_API ALobbyGameMode : public AMultiGameMode
{
	GENERATED_BODY()

public:
	ALobbyGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	void CheckPlayersReadiness();

	UFUNCTION(BlueprintCallable)
		void TravelGameMap();

private:
	int NumPlayers = 0;		
};
