// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "MainMenu.h"
#include "ServerRow.h"
#include "MenuInterface.h"
#include "MenuPlayerController.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

const uint32 UMainMenu::UnselectedIndex = 667;

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> WBP_ServerRowClass(TEXT("/Game/MenuSystem/Widgets/WBP_ServerRow"));
	ServerRowClass = (WBP_ServerRowClass.Class ? WBP_ServerRowClass.Class : nullptr);
}

void UMainMenu::SetupMainMenu()
{
	this->AddToViewport();

	FInputModeGameAndUI UIInputMode;
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

void UMainMenu::AddServersToServerList(const TArray<FServerProperty>& ServerProps)
{
	if (GetMenuInterface())
	{
		for (int32 i = 0; i < ServerProps.Num(); ++i)
		{
			ServerRowWidget = (ServerRowClass ? CreateWidget<UServerRow>(GetWorld(), ServerRowClass) : nullptr);

			if (ServerRowWidget)
			{
				ServerRowWidget->SetServerText(ServerProps[i].Name);
				ServerRowWidget->SetHostingUserText(ServerProps[i].HostedUsername);
				ServerRowWidget->SetNumPlayersText(FString::Printf(TEXT("%d/%d"), ServerProps[i].CurrentPlayers, ServerProps[i].MaxPlayers));
				ServerRowWidget->SetModeNameText(UEnum::GetValueAsString(ServerProps[i].GameMode.GetValue()));
				ServerRowWidget->Setup(this, i);
				ServerList->AddChild(ServerRowWidget);
			}
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

void UMainMenu::BackToMainMenu()
{
	if (MenuSwitcher && BackFromJoinButton->GetIsEnabled())
	{
		switch (MenuSwitcher->GetActiveWidgetIndex())
		{
		case int32(EMenuTypes::HOST_MENU):
			HostNameTextBox->SetText(FText()); // Erasing The Text
			break;
		case int32(EMenuTypes::JOIN_MENU):
			ServerList->ClearChildren();
			break;
		}

		MenuSwitcher->SetActiveWidgetIndex(int32(EMenuTypes::MAIN_MENU));
	}
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
	
	BackFromJoinButton->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);
	RefreshServersListButton->OnClicked.AddDynamic(this, &UMainMenu::OnRefreshServersListClicked);
	JoinGameButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinGameClicked);

	HostNameTextBox->OnTextCommitted.AddDynamic(this, &UMainMenu::OnHostNameTextCommitted);
	BackFromHostButton->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);
	HostGameButton->OnClicked.AddDynamic(this, &UMainMenu::OnHostGameClicked);
	SelectedGameModeButton->OnClicked.AddDynamic(this, &UMainMenu::OnSelectedGameModeClicked);

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
	DeselectIndex();
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

void UMainMenu::OnRefreshServersListClicked()
{
	ServerList->ClearChildren();

	if (MenuInterface)
	{		
		ToggleRefreshingServersSectionButtons(false);
		DeselectIndex();
		MenuInterface->RefreshServers();
	}
}

void UMainMenu::OnJoinGameClicked()
{
	if (SelectedIndex.IsSet() && GetMenuInterface())	
		GetMenuInterface()->JoinGame(SelectedIndex.GetValue());
}

void UMainMenu::OnHostGameClicked()
{
	if (GetMenuInterface())
		HostNameTextBox->GetText().IsEmpty() ?
			GetMenuInterface()->HostGame() :
			GetMenuInterface()->HostGame(HostNameTextBox->GetText().ToString());
}

void UMainMenu::OnSelectedGameModeClicked()
{	
	// Check if the next element is None. If it is assign enum variable to the first element of enum. 
	SelectedGameMode = UEnum::GetValueAsString(EGameMode(GetSelectedGameMode() + 1)) == "EGameMode_MAX" ? EGameMode(0) : EGameMode(GetSelectedGameMode() + 1); // UE automatically creates MAX element to indicate the ending of a list
	SelectedGameModeText->SetText(FText::FromString(UEnum::GetValueAsString(GetSelectedGameMode())));
	FString ModeFullName;
	switch (GetSelectedGameMode())
	{		
	case PJ:
		ModeFullName = "Platform Jumper";		
		break;
	case SV:
		ModeFullName = "Speedy Vehicle";
		break;	
	}
	SelectedGameModeButton->SetToolTipText(FText::FromString(ModeFullName));
}

void UMainMenu::OnHostNameTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
		OnHostGameClicked();
}
