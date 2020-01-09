// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "AlertBox.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UAlertBox::SetupAlertBox()
{
	this->AddToViewport();

	FInputModeUIOnly UIInputMode;
	UIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	GetWorld()->GetFirstPlayerController()->SetInputMode(UIInputMode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

void UAlertBox::TeardownAlertBox()
{
	this->RemoveFromViewport();
}

void UAlertBox::SetErrorText(const FString& Text)
{
	ErrorText->SetText(FText::FromString(Text));
}

bool UAlertBox::Initialize()
{
	if (!Super::Initialize())
		return false;

	OKButton->OnClicked.AddDynamic(this, &UAlertBox::OnOKButtonClicked);

	return true;
}

void UAlertBox::OnOKButtonClicked()
{
	TeardownAlertBox();
	MenuInterface->LoadMainMenu();
}
