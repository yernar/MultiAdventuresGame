// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "MenuInterface.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMJUMPERGAME_API UGameMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetupGameMenu();
	void TeardownGameMenu();

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		class UButton* CancelButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		class UButton* QuitButton;

	IMenuInterface* MenuInterface;

public:
	FORCEINLINE void SetMenuInterface(IMenuInterface* MenuInterface)
	{
		this->MenuInterface = MenuInterface;
	}

private:
	UFUNCTION(BlueprintCallable)
		void OnCancelButtonClicked();
	UFUNCTION(BlueprintCallable)
		void OnQuitButtonClicked();
	
};


