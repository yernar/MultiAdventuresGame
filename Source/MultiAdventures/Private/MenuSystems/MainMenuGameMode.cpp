// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "MainMenuGameMode.h"

#include "MultiGameInstance.h"
#include "MenuPlayerController.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	PlayerControllerClass = AMenuPlayerController::StaticClass();
}

void AMainMenuGameMode::StartPlay()
{
	Super::StartPlay();
	
	Cast<UMultiGameInstance>(GetWorld()->GetGameInstance())->LoadMainMenu();
}
