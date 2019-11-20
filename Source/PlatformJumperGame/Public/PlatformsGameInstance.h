// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "MenuInterface.h"
#include "OnlineSessionInterface.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMJUMPERGAME_API UPlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:
	UPlatformsGameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
		void LoadMainMenu();
	UFUNCTION(BlueprintCallable)
		void LoadGameMenu();

protected:
	virtual void Init();

private:
	UFUNCTION(Exec)
		virtual void HostGame() override;
	UFUNCTION(Exec)
		virtual void JoinGame(const FString& Address = "127.0.0.1") override;
	UFUNCTION(Exec)
		virtual void QuitToMainMenu() override;
	UFUNCTION(Exec)
		virtual void QuitFromMainMenu() override;

	void OnCreateSessionComplete(FName SessionName, bool bSuccess);
	void OnDestroySessionComplete(FName SessionName, bool bSuccess);

	// ****************************MENU VARS************************ //
	TSubclassOf<class UUserWidget> MainMenuClass;
	TSubclassOf<class UUserWidget> GameMenuClass;

	class UMainMenu* MainMenuWidget;
	class UGameMenu* GameMenuWidget;
	// ****************************MENU VARS************************ //

	IOnlineSessionPtr OSS_Interface;
	const static FName SESSION_NAME;
};
