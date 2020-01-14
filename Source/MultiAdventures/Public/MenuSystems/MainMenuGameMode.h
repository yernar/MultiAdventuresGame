// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Multi/MultiGameMode.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIADVENTURES_API AMainMenuGameMode : public AMultiGameMode
{
	GENERATED_BODY()

public:
	virtual void StartPlay() override;
};
