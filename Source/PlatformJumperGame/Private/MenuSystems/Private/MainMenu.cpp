// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "MainMenu.h"

#include "Components/Button.h"
#include "Blueprint/UserWidget.h"

void UMainMenu::SetMenuInterface(IMenuInterface* MenuInterface)
{
	this->MenuInterface = MenuInterface;
}

void UMainMenu::SetupMainMenu()
{
	this->AddToViewport();

	FInputModeUIOnly UIInputMode;
	UIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	
	GetWorld()->GetFirstPlayerController()->SetInputMode(UIInputMode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = Windows::TRUE;
}

void UMainMenu::TeardownMainMenu()
{
	this->RemoveFromViewport();

	FInputModeGameOnly GameInputMode;

	GetWorld()->GetFirstPlayerController()->SetInputMode(GameInputMode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = Windows::FALSE;
}

bool UMainMenu::Initialize()
{
	if (!Super::Initialize())
		return false;

	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OnHostButtonClicked);

	return true;
}

void UMainMenu::OnHostButtonClicked()
{
	if (MenuInterface)
	{
		MenuInterface->HostGame();
	}
}
