// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMJUMPERGAME_API UPlatformsGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPlatformsGameInstance(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void Init();

private:
	UFUNCTION(Exec)
		void HostGame();
	UFUNCTION(Exec)
		void JoinGame(const FString& Address = "127.0.0.1");

	TSubclassOf<class UUserWidget> MenuWidget;
};
