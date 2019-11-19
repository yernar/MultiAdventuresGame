// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "PlatformsGameInstance.h"
#include "PlatformTrigger.h"
#include "MainMenu.h"
#include "GameMenu.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/LocalPlayer.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

UPlatformsGameInstance::UPlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> WBP_MainMenuClass(TEXT("/Game/MenuSystem/Widgets/WBP_MainMenu"));
	MainMenuClass = (WBP_MainMenuClass.Class ? WBP_MainMenuClass.Class : nullptr);

	ConstructorHelpers::FClassFinder<UUserWidget> WBP_GameMenuClass(TEXT("/Game/MenuSystem/Widgets/WBP_GameMenu"));
	GameMenuClass = (WBP_GameMenuClass.Class ? WBP_GameMenuClass.Class : nullptr);	
}

void UPlatformsGameInstance::LoadMainMenu()
{
	MainMenuWidget = (MainMenuClass ? CreateWidget<UMainMenu>(this, MainMenuClass) : nullptr);

	if (MainMenuWidget)
	{
		MainMenuWidget->SetMenuInterface(this);
		MainMenuWidget->SetupMainMenu();
	}
}

void UPlatformsGameInstance::LoadGameMenu()
{
	GameMenuWidget = (GameMenuClass ? CreateWidget<UGameMenu>(this, GameMenuClass) : nullptr);

	if (GameMenuWidget)
	{
		GameMenuWidget->SetMenuInterface(this);
		GameMenuWidget->SetupGameMenu();
	}
}

void UPlatformsGameInstance::Init()
{
	IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
	UE_LOG(LogTemp, Warning, TEXT("It's: %s."), *OSS->GetOnlineServiceName().ToString())
	OSS_Interface = OSS->GetSessionInterface();
	UE_LOG(LogTemp, Warning, TEXT("Interface is valid: %d."), OSS_Interface.IsValid())

	if (OSS_Interface.IsValid())
	{		
		OSS_Interface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPlatformsGameInstance::OnCreateSessionComplete);
	}
}

void UPlatformsGameInstance::HostGame()
{
	if (OSS_Interface.IsValid())
	{
		FOnlineSessionSettings OSS_Settings;
		OSS_Interface->CreateSession(0, FName("WTF"), OSS_Settings);
	}	
}

void UPlatformsGameInstance::JoinGame(const FString& Address)
{
	if (!GetEngine())
		return;

	if (MainMenuWidget)
		MainMenuWidget->TeardownMainMenu();

	GetPrimaryPlayerController()->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	GetEngine()->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, FString::Printf(TEXT("Joining the %s"), *Address));
}

void UPlatformsGameInstance::QuitToMainMenu()
{
	GetPrimaryPlayerController()->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPlatformsGameInstance::QuitFromMainMenu()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UPlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool bSuccess)
{
	UE_LOG(LogTemp, Warning, TEXT("Session is creating: %d"), bSuccess)

	if (!GetEngine())
		return;

	if (MainMenuWidget)
		MainMenuWidget->TeardownMainMenu();

	GetEngine()->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("Hosting"));
	if (GetWorld()->ServerTravel("/Game/Maps/MainLevel?listen"))
		GetEngine()->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, FString::Printf(TEXT("Hosted the session")));
}
