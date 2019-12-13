// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "MenuInterface.h"
#include "OnlineSessionInterface.h" // consider removing this include

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
		virtual void HostGame(const FString& HostName) override;
	UFUNCTION(Exec)
		virtual void JoinGame(uint32 ServerIndex) override;
	UFUNCTION(Exec)
		virtual void QuitToMainMenu() override;
	UFUNCTION(Exec)
		virtual void QuitFromMainMenu() override;
	UFUNCTION(Exec)
		virtual void RefreshServers() override;

	// **************************** DELEGATES ************************ // 
	void OnCreateSessionComplete(FName SessionName, bool bSuccess);
	void OnDestroySessionComplete(FName SessionName, bool bSuccess);
	void OnFindSessionsComplete(bool bSuccess);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinSessionCompleteResult);

	// ****************************MENU VARS************************ //
	TSubclassOf<class UUserWidget> MainMenuClass;
	TSubclassOf<class UUserWidget> GameMenuClass;

	class UMainMenu* MainMenuWidget;
	class UGameMenu* GameMenuWidget;
	// ****************************MENU VARS************************ //

	TSharedPtr<FOnlineSessionSearch> SessionSearch;	
	IOnlineSessionPtr SessionInterface;
	const static FName SESSION_NAME;
	const static FName SESSION_HOST_NAME_KEY;
};
