// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MainPlayerState.generated.h"


UENUM()
enum class EPlayerReadinessStatus : uint32
{
	READY,
	NOT_READY,
	NOT_ENOUGH_PLAYERS
};

UCLASS()
class MULTIADVENTURES_API AMainPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Server, Unreliable)
		void ReadyStatus(EPlayerReadinessStatus PlayerStatus);

	UFUNCTION()
		void NotEnoughPlayersHandle();
	
	EPlayerReadinessStatus PlayerReadinessStatus = EPlayerReadinessStatus::NOT_READY;

	FTimerHandle TimerHandle;
};
