// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIADVENTURES_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void BackOneStep();

protected:
	virtual void SetupInputComponent() override;
};
