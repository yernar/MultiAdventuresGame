// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "PlatformsGameInstance.h"
#include "PlatformTrigger.h"
#include "MainMenu.h"
#include "GameMenu.h"
#include "ServerRow.h"
#include "AlertBox.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/LocalPlayer.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Engine/Engine.h" // consider removing

const FName UPlatformsGameInstance::SESSION_NAME = "GameSession";
const FName UPlatformsGameInstance::SESSION_HOST_NAME_KEY = "SessionHostName";

UPlatformsGameInstance::UPlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> WBP_MainMenuClass(TEXT("/Game/MenuSystem/Widgets/WBP_MainMenu"));
	MainMenuClass = (WBP_MainMenuClass.Class ? WBP_MainMenuClass.Class : nullptr);

	ConstructorHelpers::FClassFinder<UUserWidget> WBP_GameMenuClass(TEXT("/Game/MenuSystem/Widgets/WBP_GameMenu"));
	GameMenuClass = (WBP_GameMenuClass.Class ? WBP_GameMenuClass.Class : nullptr);

	ConstructorHelpers::FClassFinder<UUserWidget> WBP_AlertBoxClass(TEXT("/Game/MenuSystem/Widgets/WBP_AlertBox"));
	AlertBoxClass = (WBP_AlertBoxClass.Class ? WBP_AlertBoxClass.Class : nullptr);
}

void UPlatformsGameInstance::LoadMainMenu()
{
	MainMenuWidget = (MainMenuClass ? CreateWidget<UMainMenu>(this, MainMenuClass) : nullptr);

	SetFailStatus(false);

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

void UPlatformsGameInstance::LoadAlertBox(const FString& Text)
{
	AlertBoxWidget = (AlertBoxClass ? CreateWidget<UAlertBox>(this, AlertBoxClass) : nullptr);

	if (AlertBoxWidget)
	{
		AlertBoxWidget->SetMenuInterface(this);
		AlertBoxWidget->SetupAlertBox();
		AlertBoxWidget->SetErrorText(Text);
	}
}

void UPlatformsGameInstance::CheckForFailures(const FString& FailText)
{
	if (bFailStatus)
		LoadAlertBox(FailText);
	else
		LoadMainMenu();
}

void UPlatformsGameInstance::Init()
{
	IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
	SessionInterface = OSS->GetSessionInterface();

	if (SessionInterface.IsValid())
	{		
		// TODO: Remove on destroy complete delegate, there is no need for that. Destroy sessions on quitting the game(host) and maybe add extra check if session exists in host game function
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPlatformsGameInstance::OnCreateSessionComplete);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPlatformsGameInstance::OnDestroySessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPlatformsGameInstance::OnFindSessionsComplete);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPlatformsGameInstance::OnJoinSessionComplete);

		SessionSearch = MakeShareable(new FOnlineSessionSearch);
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);		
	}

	GEngine->OnNetworkFailure().AddUObject(this, &UPlatformsGameInstance::OnHandleNetworkError);
}

void UPlatformsGameInstance::Shutdown()
{
	SessionInterface->DestroySession(SESSION_NAME);
}

void UPlatformsGameInstance::GeneralErrorHandler(const FString& ErrorText)
{
	// Consider remaking it with delegates
	SetFailStatus(true);
	QuitToMainMenu();
}

void UPlatformsGameInstance::HostGame(const FString& HostName)
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;		
		SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
		SessionSettings.NumPublicConnections = 5;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SESSION_HOST_NAME_KEY, HostName, EOnlineDataAdvertisementType::Type::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);					
	}	
}

void UPlatformsGameInstance::JoinGame(uint32 ServerIndex)
{
	if (MainMenuWidget && SessionInterface.IsValid() && SessionSearch.IsValid())
	{
		MainMenuWidget->TeardownMainMenu();

		SessionInterface->JoinSession(0, FName(*MainMenuWidget->GetSelectedServer()->GetServerText()), SessionSearch->SearchResults[ServerIndex]);		
	}
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
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UPlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool bSuccess)
{
	if (MainMenuWidget)
		MainMenuWidget->TeardownMainMenu();

	if (GetWorld()->ServerTravel("/Game/Maps/Lobby?listen"))
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
	if (bSuccess && SessionSearch.IsValid() && MainMenuWidget)
	{
		TArray<FServerProperty> ServerProperties;
		for (const auto& SearchResult : SessionSearch->SearchResults)
		{
			FServerProperty ServerProperty;
			SearchResult.Session.SessionSettings.Get(SESSION_HOST_NAME_KEY, ServerProperty.Name);

			ServerProperty.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			ServerProperty.CurrentPlayers = ServerProperty.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;			
			ServerProperty.HostedUsername = SearchResult.Session.OwningUserName;
			ServerProperties.Add(ServerProperty);
		}
		MainMenuWidget->AddServersToServerList(ServerProperties);	
		MainMenuWidget->ToggleRefreshingServersSectionButtons(true);
	}
}

void UPlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinSessionCompleteResult)
{
	FString ResolvedURL;
	SessionInterface->GetResolvedConnectString(SessionName, ResolvedURL);
#if WITH_EDITOR
	switch (JoinSessionCompleteResult)
	{
		case EOnJoinSessionCompleteResult::Success:
			UE_LOG(LogTemp, Warning, TEXT("Success"))
			break;
		case EOnJoinSessionCompleteResult::SessionIsFull:
			UE_LOG(LogTemp, Warning, TEXT("SessionIsFull"))
			break;
		case EOnJoinSessionCompleteResult::SessionDoesNotExist:
			UE_LOG(LogTemp, Warning, TEXT("SessionDoesNotExist"))
			break;
		case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
			UE_LOG(LogTemp, Warning, TEXT("CouldNotRetrieveAddress"))
			break;
		case EOnJoinSessionCompleteResult::AlreadyInSession:
			UE_LOG(LogTemp, Warning, TEXT("AlreadyInSession"))
			break;
		case EOnJoinSessionCompleteResult::UnknownError:
			UE_LOG(LogTemp, Warning, TEXT("UnknownError"))
			break;
	}
#endif

	GetFirstLocalPlayerController(GetWorld())->ClientTravel(ResolvedURL, ETravelType::TRAVEL_Absolute);
}

void UPlatformsGameInstance::OnHandleNetworkError(UWorld* InWorld, UNetDriver* InNetDriver, ENetworkFailure::Type InFailureType, const FString& InString)
{
	FString FailErrorText;
	switch (InFailureType)
	{
		case ENetworkFailure::NetDriverAlreadyExists:
			FailErrorText = "NetDriverAlreadyExists";
			break;
		case ENetworkFailure::NetDriverCreateFailure:
			FailErrorText = "NetDriverAlreadyExists";
			break;
		case ENetworkFailure::NetDriverListenFailure:
			FailErrorText = "NetDriverListenFailure";
			break;
		case ENetworkFailure::ConnectionLost:
			FailErrorText = "ConnectionLost";
			break;
		case ENetworkFailure::ConnectionTimeout:
			FailErrorText = "ConnectionTimeout";
			break;
		case ENetworkFailure::FailureReceived:
			FailErrorText = "FailureReceived";
			break;
		case ENetworkFailure::OutdatedClient:
			FailErrorText = "OutdatedClient";
			break;
		case ENetworkFailure::OutdatedServer:
			FailErrorText = "OutdatedServer";
			break;
		case ENetworkFailure::PendingConnectionFailure:
			FailErrorText = "PendingConnectionFailure";
				break;
		case ENetworkFailure::NetGuidMismatch:
			FailErrorText = "NetGuidMismatch";
				break;
		case ENetworkFailure::NetChecksumMismatch:
			FailErrorText = "NetChecksumMismatch";
			break;
		default:
			FailErrorText = "UNKNOWN";
			break;
	}

	FailErrorText = InString;

	GeneralErrorHandler(FailErrorText);	
}
