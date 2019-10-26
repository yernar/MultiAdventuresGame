// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "MenuInterface.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMJUMPERGAME_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMenuInterface(IMenuInterface* MenuInterface);
	
	void SetupMainMenu();
	void TeardownMainMenu();

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		class UButton* SoloButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		class UButton* HostButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		class UButton* JoinButton;

	UFUNCTION(BlueprintCallable)
		void OnHostButtonClicked();

	IMenuInterface* MenuInterface;
};
