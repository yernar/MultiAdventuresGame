// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#include "PlatformJumperGameGameMode.h"
#include "PlatformJumperGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

APlatformJumperGameGameMode::APlatformJumperGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void APlatformJumperGameGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	numplayers++;
	
	UE_LOG(LogTemp, Warning, TEXT("We have now: %d players!"), numplayers);

	if (numplayers == 2)
		if (GetWorld()->ServerTravel("/Game/Maps/MainLevel?listen"))
			UE_LOG (LogTemp, Warning, TEXT("SUCCESS"))
		else UE_LOG(LogTemp, Warning, TEXT("NOPE"))
}

void APlatformJumperGameGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	numplayers--;
	UE_LOG(LogTemp, Warning, TEXT("We have now: %d players!"), numplayers)
}
