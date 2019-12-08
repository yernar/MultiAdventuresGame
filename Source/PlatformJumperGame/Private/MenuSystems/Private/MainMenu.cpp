// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "MainMenu.h"
#include "ServerRow.h"
#include "MenuInterface.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
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

void UMainMenu::AddServers(TArray<FString> ServerNames)
{
	if (GetMenuInterface())
	{
		for (int32 i = 0; i < ServerNames.Num(); ++i)
		{
			ServerRowWidget = (ServerRowClass ? CreateWidget<UServerRow>(GetWorld(), ServerRowClass) : nullptr);
			ServerRowWidget->SetServerText(ServerNames[i]);
			ServerRowWidget->Setup(this, i);
			ServerList->AddChild(ServerRowWidget);
			// TODO: After joining a game, delete all these widgets
		}		
	}
}

const UServerRow * UMainMenu::GetSelectedServer() const
{
	if (SelectedIndex.IsSet())
	{
		ServerList->GetChildAt(SelectedIndex.GetValue());
	}
	return nullptr;
}

void UMainMenu::Tick(FGeometry MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);
	
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
	if (GetMenuInterface())
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
	if (GetMenuInterface())
	{
		MenuInterface->HostGame();
	}
}

void UMainMenu::OnQuitFromMainButtonClicked()
{
	if (GetMenuInterface())
	{
		MenuInterface->QuitFromMainMenu();
	}
}

void UMainMenu::OnBackButtonClicked()
{	
	ServerList->ClearChildren();
	UE_LOG(LogTemp, Warning, TEXT("Now: %d"), ServerList->GetChildrenCount());
	// IPTextBox->SetText(FText());
	//MenuSwitcher->SetActiveWidgetIndex(int32(EMenuTypes::MAIN_MENU));
}

void UMainMenu::OnJoinGameButtonClicked()
{
	AddServers({ "SAP", "WTF" });
	UE_LOG(LogTemp, Warning, TEXT("Now: %d"), ServerList->GetChildrenCount());
	if (SelectedIndex.IsSet() && GetMenuInterface())
	{
		UE_LOG(LogTemp, Warning, TEXT("SELECTED INDEX: %d"), SelectedIndex.GetValue());
		
		MenuInterface->JoinGame(SelectedIndex.GetValue()/*IPTextBox->GetText().ToString()*/);
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SELECTED INDEX:ERROR"))
	}
}