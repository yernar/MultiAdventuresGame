// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

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

	void AddServers(TArray<struct FServerProperty> ServerNames);
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
	UFUNCTION(BlueprintCallable)
		void OnHostMenuSwitcherClicked();
	UFUNCTION(BlueprintCallable)
		void OnJoinMenuSwitcherClicked();
	UFUNCTION(BlueprintCallable)
		void OnSoloButtonClicked();
	UFUNCTION(BlueprintCallable)
		void OnQuitFromMainClicked();

	UFUNCTION(BlueprintCallable)
		void OnBackFromJoinClicked();
	UFUNCTION(BlueprintCallable)
		void OnJoinGameClicked();	
};
