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

const FName UPlatformsGameInstance::SESSION_NAME = "PLATFORMS";

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
		// TODO: Remove on destroy complete delegate, there is no need for this. Destroy sessions on quitting the game(host) and maybe add extra check if session exists in host game function
		OSS_Interface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPlatformsGameInstance::OnCreateSessionComplete);
		OSS_Interface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPlatformsGameInstance::OnDestroySessionComplete);
		OSS_Interface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPlatformsGameInstance::OnFindSessionsComplete);

		SessionSearch = MakeShareable(new FOnlineSessionSearch);

		OSS_Interface->FindSessions(0, SessionSearch.ToSharedRef());
		
	}
}

void UPlatformsGameInstance::HostGame()
{
	if (OSS_Interface.IsValid())
	{
		FNamedOnlineSession* OnlineSession = OSS_Interface->GetNamedSession(SESSION_NAME);
		if (OnlineSession)
			OSS_Interface->DestroySession(SESSION_NAME);

		FOnlineSessionSettings OSS_Settings;
		OSS_Interface->CreateSession(0, SESSION_NAME, OSS_Settings);
					
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
	OSS_Interface->DestroySession(SESSION_NAME);
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

void UPlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool bSuccess)
{
	(GetEngine() ?
		GetEngine()->AddOnScreenDebugMessage(-1, 1.5f, FColor::Blue, FString::Printf(TEXT("Session is destroyed")))
		: GetEngine()->AbortInsideMemberFunction());
}

void UPlatformsGameInstance::OnFindSessionsComplete(bool bSuccess)
{
	(GetEngine() ?
		GetEngine()->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, FString::Printf(TEXT("Session is found")))
		: GetEngine()->AbortInsideMemberFunction());
}
