// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "ServerRow.h"
#include "MainMenu.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

const FLinearColor UServerRow::DefaultTextColor = FLinearColor::White;

bool UServerRow::Initialize()
{
	if (!Super::Initialize())
		return false;

	RowButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
	RowButton->OnHovered.AddDynamic(this, &UServerRow::OnHovered);
	RowButton->OnUnhovered.AddDynamic(this, &UServerRow::OnUnhovored);

	return true;
}

void UServerRow::SetServerText(const FString& Text)
{
	GetServerName()->SetText(FText::FromString(Text));
}

const FString& UServerRow::GetServerText() const
{
	return GetServerName()->GetText().ToString();	
}

void UServerRow::SetHostingUserText(const FString& Text)
{
	// TODO: Refactor reusable function which accepts ENUM
	GetHostingUser()->SetText(FText::FromString(Text));
}

const FString& UServerRow::GetHostingUserText() const
{
	return GetHostingUser()->GetText().ToString();
}

void UServerRow::SetNumPlayersText(const FString& Text)
{
	GetNumPlayers()->SetText(FText::FromString(Text));;
}

const FString& UServerRow::GetNumPlayersText() const
{
	return GetNumPlayers()->GetText().ToString();
}

void UServerRow::OnSelected()
{
	UpdateTextColor(FLinearColor::Green);
}

void UServerRow::OnUnselected()
{
	SetTextDefaultColor();
}

void UServerRow::OnClicked()
{
	ParentMenu->SelectIndex(Index);

	ParentMenu->UpdateSelectedServerColor();
}

void UServerRow::OnHovered()
{
	if (Index != ParentMenu->GetSelectedIndex())
		UpdateTextColor(FLinearColor::Yellow);
	else
		UpdateTextColor(FLinearColor::Red);
}

void UServerRow::OnUnhovored()
{
	if (Index != ParentMenu->GetSelectedIndex())
		SetTextDefaultColor();
	else
		UpdateTextColor(FLinearColor::Green);
}

void UServerRow::UpdateTextColor(const FLinearColor& Color)
{
	ServerName->SetColorAndOpacity(FSlateColor(Color));
	NumPlayers->SetColorAndOpacity(FSlateColor(Color));
	HostingUser->SetColorAndOpacity(FSlateColor(Color));
}

void UServerRow::SetTextDefaultColor()
{
	ServerName->SetColorAndOpacity(FSlateColor(DefaultTextColor));
	NumPlayers->SetColorAndOpacity(FSlateColor(DefaultTextColor));
	HostingUser->SetColorAndOpacity(FSlateColor(DefaultTextColor));
}
