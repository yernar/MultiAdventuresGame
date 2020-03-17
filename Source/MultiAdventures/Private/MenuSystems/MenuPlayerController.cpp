// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "MenuPlayerController.h"

#include "MainMenu.h"
#include "MultiGameInstance.h"

void AMenuPlayerController::BackOneStep()
{
	Cast<UMultiGameInstance>(GetWorld()->GetGameInstance())->GetMainMenuWidget()->BackToMainMenu();
}

void AMenuPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);
	InputComponent->BindAction("BackOneStep", IE_Pressed, this, &AMenuPlayerController::BackOneStep); // insted of this try to replace it with AMainMenu
}