// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "MainMenuGameMode.h"

#include "MultiGameInstance.h"

void AMainMenuGameMode::StartPlay()
{
	Super::StartPlay();

	Cast<UMultiGameInstance>(GetWorld()->GetGameInstance())->LoadMainMenu();
}
