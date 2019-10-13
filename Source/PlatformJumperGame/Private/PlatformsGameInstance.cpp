// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "PlatformsGameInstance.h"

UPlatformsGameInstance::UPlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
}

void UPlatformsGameInstance::Init()
{
}

void UPlatformsGameInstance::HostGame()
{
	if (!GetEngine())
		return;

	GetEngine()->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, TEXT("Hostinge"));
	if (GetWorld()->ServerTravel("/Game/Maps/MainLevel?listen"))
		GetEngine()->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("Hosted the session")));
}

void UPlatformsGameInstance::JoinGame(const FString& Address)
{
	if (!GetEngine())
		return;

	GetEngine()->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("Joining the %s"), *Address));
}
