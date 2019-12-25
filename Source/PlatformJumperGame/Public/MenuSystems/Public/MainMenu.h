// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UENUM()
enum class EMenuTypes : uint32
{
	MAIN_MENU,
	JOIN_MENU,
	HOST_MENU
};

UCLASS()
class PLATFORMJUMPERGAME_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetupMainMenu();
	void TeardownMainMenu();

	void AddServersToServerList(const TArray<struct FServerProperty>& ServerNames);
	const class UServerRow* GetSelectedServer() const;

	void UpdateSelectedServerColor();

	static const uint32 UnselectedIndex;

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
	// ******************************************************* Main Menu Page Components ****************************************************************** //

	// ******************************************************* Join Menu Page Components ****************************************************************** //
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		UButton* BackFromJoinButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		UButton* JoinGameButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = List)
		class UPanelWidget* ServerList;
	// ******************************************************* Join Menu Page Components ****************************************************************** //

	// ******************************************************* Host Menu Page Components ****************************************************************** //
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		UButton* BackFromHostButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		UButton* HostGameButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = List)
		class UEditableTextBox* HostNameTextBox;
	// ******************************************************* Host Menu Page Components ****************************************************************** //

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		class UWidgetSwitcher* MenuSwitcher;

	class IMenuInterface* MenuInterface;

	TSubclassOf<UServerRow> ServerRowClass;

	TOptional<uint32> SelectedIndex;

	UServerRow* ServerRowWidget;

public:
	FORCEINLINE void SetMenuInterface(IMenuInterface* Interface) { MenuInterface = Interface; }
	FORCEINLINE IMenuInterface* GetMenuInterface() { return MenuInterface; }
	FORCEINLINE uint32 GetSelectedIndex() { return (SelectedIndex.IsSet() ? SelectedIndex.GetValue() : UnselectedIndex); }
	FORCEINLINE void SelectIndex(uint32 Index) { SelectedIndex = Index; }

private:
	UFUNCTION()
		void OnHostMenuSwitcherClicked();
	UFUNCTION()
		void OnJoinMenuSwitcherClicked();
	UFUNCTION()
		void OnSoloButtonClicked();
	UFUNCTION()
		void OnQuitFromMainClicked();

	UFUNCTION()
		void OnBackFromJoinClicked();
	UFUNCTION()
		void OnJoinGameClicked();	

	UFUNCTION()
		void OnBackFromHostClicked();
	UFUNCTION()
		void OnHostGameClicked();
	UFUNCTION()
		void OnHostNameTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
