// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "GameMenu.h"
#include "PlatformJumperGameCharacter.h"

#include "Components/Button.h"
#include "Blueprint/UserWidget.h"

void UGameMenu::SetupGameMenu()
{
	this->AddToViewport();

	FInputModeUIOnly UIInputMode;
	UIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	GetWorld()->GetFirstPlayerController()->SetInputMode(UIInputMode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = Windows::TRUE;
}

void UGameMenu::TeardownGameMenu()
{
	this->RemoveFromViewport();

	FInputModeGameOnly GameInputMode;

	GetWorld()->GetFirstPlayerController()->SetInputMode(GameInputMode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = Windows::FALSE;
}

bool UGameMenu::Initialize()
{
	if (!Super::Initialize())
		return false;

	CancelButton->OnClicked.AddDynamic(this, &UGameMenu::OnCancelButtonClicked);
	QuitButton->OnClicked.AddDynamic(this, &UGameMenu::OnQuitButtonClicked);
	return true;
}

void UGameMenu::OnCancelButtonClicked()
{
	TeardownGameMenu();

	Cast<APlatformJumperGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->TogglePlayerCharacterInput(true);
}

void UGameMenu::OnQuitButtonClicked()
{
	TeardownGameMenu();

	if (MenuInterface)
	{
		MenuInterface->QuitToMainMenu();
	}
}
