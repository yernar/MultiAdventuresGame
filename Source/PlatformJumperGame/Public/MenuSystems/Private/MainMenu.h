// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "MenuInterface.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UENUM()
enum class EMenuTypes : int32
{
	MAIN_MENU,
	JOIN_MENU,
};

UCLASS()
class PLATFORMJUMPERGAME_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetupMainMenu();
	void TeardownMainMenu();

protected:
	virtual bool Initialize() override;

private:
	// ******************************************************* First Menu Page Components ****************************************************************** //
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		class UButton* SoloButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		UButton* HostButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		UButton* JoinMenuSwitcherButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		UButton* QuitFromMainButton;
	// ******************************************************* First Menu Page Components ****************************************************************** //

	// ******************************************************* Second Menu Page Components ****************************************************************** //
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		UButton* BackButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		UButton* JoinGameButton;
	 UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = List)
		class UPanelWidget* ServerList;
	// ******************************************************* Second Menu Page Components ****************************************************************** //

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		class UWidgetSwitcher* MenuSwitcher;

	IMenuInterface* MenuInterface;

	TSubclassOf<class UServerRow> ServerRowClass;

	UServerRow* ServerRowWidget;

public:
	FORCEINLINE void SetMenuInterface(IMenuInterface* Interface)
	{
		MenuInterface = Interface;
	}

private:
	UFUNCTION(BlueprintCallable)
		void OnHostButtonClicked();
	UFUNCTION(BlueprintCallable)
		void OnJoinMenuSwitcherButtonClicked();
	UFUNCTION(BlueprintCallable)
		void OnSoloButtonClicked();
	UFUNCTION(BlueprintCallable)
		void OnQuitFromMainButtonClicked();

	UFUNCTION(BlueprintCallable)
		void OnBackButtonClicked();
	UFUNCTION(BlueprintCallable)
		void OnJoinGameButtonClicked();
	
	
};
