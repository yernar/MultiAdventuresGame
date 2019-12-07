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
	SessionInterface = OSS->GetSessionInterface();
	UE_LOG(LogTemp, Warning, TEXT("Interface is valid: %d."), SessionInterface.IsValid())

	if (SessionInterface.IsValid())
	{		
		// TODO: Remove on destroy complete delegate, there is no need for this. Destroy sessions on quitting the game(host) and maybe add extra check if session exists in host game function
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPlatformsGameInstance::OnCreateSessionComplete);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPlatformsGameInstance::OnDestroySessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPlatformsGameInstance::OnFindSessionsComplete);

		SessionSearch = MakeShareable(new FOnlineSessionSearch);
		SessionSearch->bIsLanQuery = true;
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		
	}
}

void UPlatformsGameInstance::HostGame()
{
	if (SessionInterface.IsValid())
	{
		FNamedOnlineSession* OnlineSession = SessionInterface->GetNamedSession(SESSION_NAME);

		if (OnlineSession)
			SessionInterface->DestroySession(SESSION_NAME);

		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = true;
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);					
	}	
}

void UPlatformsGameInstance::JoinGame(const FString& Address)
{
	if (!GetEngine())
		return;

	if (MainMenuWidget)
	{
	}//RefreshServers();
		// MainMenuWidget->TeardownMainMenu();

	/*GetPrimaryPlayerController()->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	GetEngine()->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, FString::Printf(TEXT("Joining the %s"), *Address));*/
}

void UPlatformsGameInstance::QuitToMainMenu()
{
	GetPrimaryPlayerController()->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
	SessionInterface->DestroySession(SESSION_NAME);
}

void UPlatformsGameInstance::QuitFromMainMenu()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UPlatformsGameInstance::RefreshServers()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
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
	// TODO: Needs <Refactoring>
	if (bSuccess && SessionSearch.IsValid() && MainMenuWidget)
	{
		TArray<FString> Servers;
		GetEngine()->AddOnScreenDebugMessage(-1, 1.5f, FColor::Blue, FString::Printf( TEXT("Finished searching for sessions") ));
		for (const auto& Session : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Session is found: %s"), *Session.GetSessionIdStr());
			Servers.Add(*Session.GetSessionIdStr());
		}
		MainMenuWidget->AddServers(Servers);
	}
}
