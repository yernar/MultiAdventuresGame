// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "PlatformsGameInstance.h"
#include "PlatformTrigger.h"

#include "Blueprint/UserWidget.h"

UPlatformsGameInstance::UPlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> WBP_WIdgetClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	MenuClass = (WBP_WIdgetClass.Class ? WBP_WIdgetClass.Class : nullptr);
}

void UPlatformsGameInstance::Init()
{
}

void UPlatformsGameInstance::HostGame()
{
	if (!GetEngine())
		return;

	GetEngine()->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("Hosting"));
	if (GetWorld()->ServerTravel("/Game/Maps/MainLevel?listen"))
		GetEngine()->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, FString::Printf(TEXT("Hosted the session")));
}

void UPlatformsGameInstance::JoinGame(const FString& Address)
{
	if (!GetEngine())
		return;
	GetPrimaryPlayerController()->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	GetEngine()->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, FString::Printf(TEXT("Joining the %s"), *Address));
}

void UPlatformsGameInstance::LoadMainMenu()
{
	MenuWidget = (MenuClass ? CreateWidget<UUserWidget>(this, MenuClass) : nullptr);

	if (MenuWidget)
		MenuWidget->AddToViewport();
}
