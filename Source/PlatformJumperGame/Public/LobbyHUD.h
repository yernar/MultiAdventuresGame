// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMJUMPERGAME_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()
public:
	ALobbyHUD(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void DrawHUD() override;
	
private:
	UFont* FredokaFont;
};
