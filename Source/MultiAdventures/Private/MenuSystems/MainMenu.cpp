// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "MainMenu.h"
#include "ServerRow.h"
#include "MenuInterface.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

const uint32 UMainMenu::UnselectedIndex = 667; // TODO: WTF??

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
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	MainMenuLoaded.Broadcast();
}

void UMainMenu::TeardownMainMenu()
{
	this->RemoveFromViewport();

	FInputModeGameOnly GameInputMode;

	GetWorld()->GetFirstPlayerController()->SetInputMode(GameInputMode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
}

void UMainMenu::AddServersToServerList(const TArray<FServerProperty>& ServerNames)
{
	if (GetMenuInterface())
	{
		for (int32 i = 0; i < ServerNames.Num(); ++i)
		{
			ServerRowWidget = (ServerRowClass ? CreateWidget<UServerRow>(GetWorld(), ServerRowClass) : nullptr);

			if (ServerRowWidget)
			{
				ServerRowWidget->SetServerText(ServerNames[i].Name);
				ServerRowWidget->SetHostingUserText(ServerNames[i].HostedUsername);
				ServerRowWidget->SetNumPlayersText(FString::Printf(TEXT("%d/%d"), ServerNames[i].CurrentPlayers, ServerNames[i].MaxPlayers));
				ServerRowWidget->Setup(this, i);
				ServerList->AddChild(ServerRowWidget);
			}
			// TODO: After joining a game, delete all these widgets
		}		
	}
}

const UServerRow* UMainMenu::GetSelectedServer() const
{
	return ( SelectedIndex.IsSet() ?
		Cast<UServerRow> (ServerList->GetChildAt(SelectedIndex.GetValue())) :
		nullptr );
}

void UMainMenu::UpdateSelectedServerColor()
{
	if (!SelectedIndex.IsSet())
		return;

	for (auto* Server : ServerList->GetAllChildren())
	{
		UServerRow* ServerRow = Cast<UServerRow>(Server);
		if (SelectedIndex.GetValue() == ServerRow->GetIndex())
			ServerRow->OnSelected();
		else
			ServerRow->OnUnselected();
	}
}

void UMainMenu::ToggleRefreshingServersSectionButtons(bool bIsEnabled)
{
	BackFromJoinButton->SetIsEnabled(bIsEnabled);
	RefreshServersListButton->SetIsEnabled(bIsEnabled);
	JoinGameButton->SetIsEnabled(bIsEnabled);
}

bool UMainMenu::Initialize()
{
	if (!Super::Initialize())
		return false;

	MenuSwitcher->SetActiveWidgetIndex(int32(EMenuTypes::MAIN_MENU));
	SoloButton->OnClicked.AddDynamic(this, &UMainMenu::OnSoloButtonClicked);
	HostMenuSwitcherButton->OnClicked.AddDynamic(this, &UMainMenu::OnHostMenuSwitcherClicked);
	JoinMenuSwitcherButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinMenuSwitcherClicked);
	QuitFromMainButton->OnClicked.AddDynamic(this, &UMainMenu::OnQuitFromMainClicked);

	SoloButton->SetIsEnabled(false);
	
	BackFromJoinButton->OnClicked.AddDynamic(this, &UMainMenu::OnBackFromJoinClicked);
	RefreshServersListButton->OnClicked.AddDynamic(this, &UMainMenu::OnRefreshServersListClicked);
	JoinGameButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinGameClicked);

	HostNameTextBox->OnTextCommitted.AddDynamic(this, &UMainMenu::OnHostNameTextCommitted);
	BackFromHostButton->OnClicked.AddDynamic(this, &UMainMenu::OnBackFromHostClicked);
	HostGameButton->OnClicked.AddDynamic(this, &UMainMenu::OnHostGameClicked);

	/* Solo is Not Available R8 now */
	SoloButton->SetIsEnabled(false);

	return true;
}

void UMainMenu::OnHostMenuSwitcherClicked()
{
	MenuSwitcher->SetActiveWidgetIndex(int32(EMenuTypes::HOST_MENU));
}

void UMainMenu::OnJoinMenuSwitcherClicked()
{
	MenuSwitcher->SetActiveWidgetIndex(int32(EMenuTypes::JOIN_MENU));
	MenuInterface->RefreshServers();

	/* Wait Until Session Finishes Searching for Available Servers */
	ToggleRefreshingServersSectionButtons(false);
}

void UMainMenu::OnSoloButtonClicked()
{
	// TODO
}

void UMainMenu::OnQuitFromMainClicked()
{
	if (GetMenuInterface())
		GetMenuInterface()->QuitFromMainMenu();
}

void UMainMenu::OnBackFromJoinClicked()
{
	MenuSwitcher->SetActiveWidgetIndex(int32(EMenuTypes::MAIN_MENU));
	ServerList->ClearChildren();
}

void UMainMenu::OnRefreshServersListClicked()
{
	ServerList->ClearChildren();

	if (MenuInterface)
	{		
		ToggleRefreshingServersSectionButtons(false);
		MenuInterface->RefreshServers();
	}
}

void UMainMenu::OnJoinGameClicked()
{
	if (SelectedIndex.IsSet() && GetMenuInterface())	
		GetMenuInterface()->JoinGame(SelectedIndex.GetValue());
}

void UMainMenu::OnBackFromHostClicked()
{
	HostNameTextBox->SetText(FText()); // Erasing The Text
	MenuSwitcher->SetActiveWidgetIndex(int32(EMenuTypes::MAIN_MENU));
}

void UMainMenu::OnHostGameClicked()
{
	if (GetMenuInterface())
		HostNameTextBox->GetText().IsEmpty() ?
			GetMenuInterface()->HostGame() :
			GetMenuInterface()->HostGame(HostNameTextBox->GetText().ToString());
}

void UMainMenu::OnHostNameTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
		OnHostGameClicked();
}
