// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PLATFORMJUMPERGAME_API IMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void HostGame() = 0;
	virtual void JoinGame(const FString& Address) = 0;
	virtual void QuitToMainMenu() = 0;
	virtual void QuitFromMainMenu() = 0;
	virtual void RefreshServers() = 0;
};
