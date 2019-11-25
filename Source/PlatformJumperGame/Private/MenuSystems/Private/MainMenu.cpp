// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "MainMenu.h"
#include "ServerRow.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Blueprint/UserWidget.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> WBP_ServerRowClass(TEXT("/Game/MenuSystem/Widgets/WBP_ServerRow"));
	ServerRowClass = (WBP_ServerRowClass.Class ? WBP_ServerRowClass.Class : nullptr);
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

	MenuSwitcher->SetActiveWidgetIndex(int32(EMenuTypes::MAIN_MENU));
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OnHostButtonClicked);
	JoinMenuSwitcherButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinMenuSwitcherButtonClicked);
	QuitFromMainButton->OnClicked.AddDynamic(this, &UMainMenu::OnQuitFromMainButtonClicked);

	SoloButton->OnClicked.AddDynamic(this, &UMainMenu::OnSoloButtonClicked);
	BackButton->OnClicked.AddDynamic(this, &UMainMenu::OnBackButtonClicked);
	JoinGameButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinGameButtonClicked);

	SoloButton->SetIsEnabled(false);

	return true;
}

void UMainMenu::OnHostButtonClicked()
{
	if (MenuInterface)
	{
		MenuInterface->HostGame();
	}
}

void UMainMenu::OnJoinMenuSwitcherButtonClicked()
{
	MenuSwitcher->SetActiveWidgetIndex(int32(EMenuTypes::JOIN_MENU));
}

void UMainMenu::OnSoloButtonClicked()
{
	// TODO: Make a solo game mode & function related with solo gameplay.
	if (MenuInterface)
	{
		MenuInterface->HostGame();
	}
}

void UMainMenu::OnQuitFromMainButtonClicked()
{
	MenuInterface->QuitFromMainMenu();
}

void UMainMenu::OnBackButtonClicked()
{	
	// IPTextBox->SetText(FText());
	MenuSwitcher->SetActiveWidgetIndex(int32(EMenuTypes::MAIN_MENU));
}

void UMainMenu::OnJoinGameButtonClicked()
{
	if (MenuInterface)
	{
		ServerRowWidget = (ServerRowClass ? CreateWidget<UServerRow>(this, ServerRowClass) : nullptr);
		ServerList->AddChild(ServerRowWidget);
		// MenuInterface->JoinGame(IPTextBox->GetText().ToString());
	}
}
