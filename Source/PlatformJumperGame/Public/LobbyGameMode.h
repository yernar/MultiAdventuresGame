// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlatformJumperGameGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMJUMPERGAME_API ALobbyGameMode : public APlatformJumperGameGameMode
{
	GENERATED_BODY()

public:
	ALobbyGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	UFUNCTION(BlueprintCallable)
		void TravelGameMap();

private:
	int NumPlayers = 0;

	FTimerHandle TimerHandle;
	
};
