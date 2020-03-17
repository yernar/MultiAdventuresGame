// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMainMenuLoaded);

UENUM()
enum class EMenuTypes : uint32
{
	MAIN_MENU,
	JOIN_MENU,
	HOST_MENU
};

UENUM()
enum EGameMode
{
	PJ, // Platform Jumper Game Mode
	SV  // Speedy Vehicle Game Mode
};

UCLASS()
class MULTIADVENTURES_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetupMainMenu();
	void TeardownMainMenu();

	void AddServersToServerList(const TArray<struct FServerProperty>& ServerNames);
	const class UServerRow* GetSelectedServer() const;

	void UpdateSelectedServerColor();
	void ToggleRefreshingServersSectionButtons(bool bIsEnabled);

	static const uint32 UnselectedIndex;

	UPROPERTY(BlueprintAssignable, Category = Delegate)
		FOnMainMenuLoaded MainMenuLoaded;

protected:
	virtual bool Initialize() override;

private:
	// ******************************************************* Main Menu Page Components ****************************************************************** //
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		class UButton* SoloButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		UButton* HostMenuSwitcherButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		UButton* JoinMenuSwitcherButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		UButton* QuitFromMainButton;
	// ******************************************************* ************************* ****************************************************************** //

	// ******************************************************* Join Menu Page Components ****************************************************************** //
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		UButton* BackFromJoinButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		UButton* RefreshServersListButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		UButton* JoinGameButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = List)
		class UPanelWidget* ServerList;
	// ******************************************************* ************************* ****************************************************************** //

	// ******************************************************* Host Menu Page Components ****************************************************************** //
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		UButton* BackFromHostButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		UButton* HostGameButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		UButton* SelectedGameModeButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = List)
		class UEditableTextBox* HostNameTextBox;	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Text)
		class UTextBlock* SelectedGameModeText;
	// ******************************************************* ************************* ****************************************************************** //

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		class UWidgetSwitcher* MenuSwitcher;

	class IMenuInterface* MenuInterface;

	TSubclassOf<UServerRow> ServerRowClass;

	TOptional<uint32> SelectedIndex;

	UServerRow* ServerRowWidget;

	static TEnumAsByte<EGameMode> SelectedGameMode;

public:
	FORCEINLINE void SetMenuInterface(IMenuInterface* Interface) { MenuInterface = Interface; }
	FORCEINLINE IMenuInterface* GetMenuInterface() { return MenuInterface; }
	FORCEINLINE uint32 GetSelectedIndex() { return (SelectedIndex.IsSet() ? SelectedIndex.GetValue() : UnselectedIndex); }
	FORCEINLINE void SelectIndex(uint32 Index) { SelectedIndex = Index; }
	FORCEINLINE static EGameMode GetSelectedGameMode() { return SelectedGameMode; }

private:
	UFUNCTION()
		void OnHostMenuSwitcherClicked();
	UFUNCTION()
		void OnJoinMenuSwitcherClicked();
	UFUNCTION()
		void OnSoloButtonClicked();
	UFUNCTION()
		void OnQuitFromMainClicked();

	/* REMOVED: See BackToMainMenu()
	 UFUNCTION()
		void OnBackFromJoinClicked();*/
	UFUNCTION()
		void OnRefreshServersListClicked();
	UFUNCTION()
		void OnJoinGameClicked();	

	/* REMOVED: See BackToMainMenu()
	 UFUNCTION()
		void OnBackFromHostClicked();*/
	UFUNCTION()
		void OnHostGameClicked();
	UFUNCTION()
		void OnSelectedGameModeClicked();
	UFUNCTION()
		void OnHostNameTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);		

	// Step Back From Other Panels to Main Menu Panel
	UFUNCTION()
		void BackToMainMenu();

	friend class AMenuPlayerController;
};
