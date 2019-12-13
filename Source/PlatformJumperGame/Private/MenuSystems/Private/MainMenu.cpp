// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "MainMenu.h"
#include "ServerRow.h"
#include "MenuInterface.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

const uint32 UMainMenu::UnselectedIndex = 667;

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

void UMainMenu::AddServers(TArray<FServerProperty> ServerNames)
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
	JoinGameButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinGameClicked);

	HostNameTextBox->OnTextCommitted.AddDynamic(this, &UMainMenu::OnHostNameTextCommitted);
	BackFromHostButton->OnClicked.AddDynamic(this, &UMainMenu::OnBackFromHostClicked);
	HostGameButton->OnClicked.AddDynamic(this, &UMainMenu::OnHostGameClicked);
	

	return true;
}

void UMainMenu::OnHostMenuSwitcherClicked()
{
	MenuSwitcher->SetActiveWidgetIndex(int32(EMenuTypes::HOST_MENU));
}

void UMainMenu::OnJoinMenuSwitcherClicked()
{
	MenuSwitcher->SetActiveWidgetIndex(int32(EMenuTypes::JOIN_MENU));
}

void UMainMenu::OnSoloButtonClicked()
{
	// TODO: Make a solo game mode & function related with solo gameplay.
	if (GetMenuInterface())
	{
		GetMenuInterface()->HostGame();
	}
}

void UMainMenu::OnQuitFromMainClicked()
{
	if (GetMenuInterface())
	{
		GetMenuInterface()->QuitFromMainMenu();
	}
}

void UMainMenu::OnBackFromJoinClicked()
{
	MenuSwitcher->SetActiveWidgetIndex(int32(EMenuTypes::MAIN_MENU));
	ServerList->ClearChildren();
}

void UMainMenu::OnJoinGameClicked()
{
	/*AddServers({ "SAP", "WTF" });
	UE_LOG(LogTemp, Warning, TEXT("Now: %d"), ServerList->GetChildrenCount());*/
	if (SelectedIndex.IsSet() && GetMenuInterface())
	{
		UE_LOG(LogTemp, Warning, TEXT("SELECTED INDEX: %d"), SelectedIndex.GetValue());
		
		GetMenuInterface()->JoinGame(SelectedIndex.GetValue()/*IPTextBox->GetText().ToString()*/);
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SELECTED INDEX:ERROR"))
	}
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
		OnHostGameClicked();;
}
