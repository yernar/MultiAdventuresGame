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
	// ******************************************************* First Menu Page Components ****************************************************************** //

	// ******************************************************* Second Menu Page Components ****************************************************************** //
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		UButton* BackButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		UButton* JoinGameButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		class UEditableTextBox* IPTextBox;
	// ******************************************************* Second Menu Page Components ****************************************************************** //

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		class UWidgetSwitcher* MenuSwitcher;

	IMenuInterface* MenuInterface;

public:
	FORCEINLINE void SetMenuInterface(IMenuInterface* MenuInterface)
	{
		this->MenuInterface = MenuInterface;
	}

private:
	UFUNCTION(BlueprintCallable)
		void OnHostButtonClicked();
	UFUNCTION(BlueprintCallable)
		void OnJoinMenuSwitcherButtonClicked();
	UFUNCTION(BlueprintCallable)
		void OnSoloButtonClicked();

	UFUNCTION(BlueprintCallable)
		void OnBackButtonClicked();
	UFUNCTION(BlueprintCallable)
		void OnJoinGameButtonClicked();
	
	
};
