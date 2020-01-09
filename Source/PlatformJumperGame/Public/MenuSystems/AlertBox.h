// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "MenuInterface.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AlertBox.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMJUMPERGAME_API UAlertBox : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetupAlertBox();
	void TeardownAlertBox();

	void SetErrorText(const FString& Text);

protected:
	virtual bool Initialize() override;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		class UTextBlock* ErrorText;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = Buttons)
		class UButton* OKButton;

	IMenuInterface* MenuInterface;

public:
	FORCEINLINE void SetMenuInterface(IMenuInterface* MenuInterface)
	{
		this->MenuInterface = MenuInterface;
	}

private:
	UFUNCTION(BlueprintCallable)
		void OnOKButtonClicked();
};
