// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "MenuInterface.h"
#include "OnlineSessionInterface.h" // consider removing this include

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MultiGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTIADVENTURES_API UMultiGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:
	UMultiGameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
		void LoadMainMenu();

	void LoadGameMenu();	
	void LoadAlertBox(const FString& Text);

	/*FORCEINLINE TArray<FServerProperty>* GetServerPorperties() { return ServerProperties; }*/

protected:
	virtual void Init();
	virtual void Shutdown();

private:
	UFUNCTION()
		virtual void HostGame(const FString& HostName) override;
	UFUNCTION()
		virtual void JoinGame(uint32 ServerIndex) override;
	UFUNCTION()
		virtual void QuitToMainMenu() override;
	UFUNCTION()
		virtual void QuitFromMainMenu() override;
	UFUNCTION()
		virtual void RefreshServers() override;

	UFUNCTION()
		void GeneralErrorHandler();

	// **************************** DELEGATES ************************ // 
	void OnCreateSessionComplete(FName SessionName, bool bSuccess);
	void OnDestroySessionComplete(FName SessionName, bool bSuccess);
	void OnFindSessionsComplete(bool bSuccess);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinSessionCompleteResult);
	void OnHandleNetworkError(UWorld* InWorld, UNetDriver* InNetDriver, ENetworkFailure::Type InFailureType, const FString& InString);
	void OnHandleTravelFailure(UWorld* InWorld, ETravelFailure::Type InFailureType, const FString& InString);
	// **************************** DELEGATES ************************ // 

	// ****************************MENU VARS************************ //
	TSubclassOf<class UUserWidget> MainMenuClass;
	TSubclassOf<UUserWidget> GameMenuClass;
	TSubclassOf<UUserWidget> AlertBoxClass;

	class UMainMenu* MainMenuWidget;
	class UGameMenu* GameMenuWidget;
	class UAlertBox* AlertBoxWidget;
	// ****************************MENU VARS************************ //
	
	// **************************** OSS ************************ //
	TSharedPtr<FOnlineSessionSearch> SessionSearch;	
	IOnlineSessionPtr SessionInterface;
	const static FName SESSION_NAME;
	const static FName SESSION_HOST_NAME_KEY;
	// **************************** OSS ************************ //

	TOptional<FString> ErrorText;
};