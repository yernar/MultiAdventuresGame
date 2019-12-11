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
	GetServerName()->SetText(FText::FromString(Text));;
}

const FString& UServerRow::GetServerText() const
{
	return GetServerName()->GetText().ToString();	
}

void UServerRow::OnSelected()
{
	UpdateTextColor(ServerName, FLinearColor::Green);
	UpdateTextColor(NumPlayers, FLinearColor::Green);
}

void UServerRow::OnUnselected()
{
	SetTextDefaultColor(ServerName);
	SetTextDefaultColor(NumPlayers);
}

void UServerRow::OnClicked()
{
	ParentMenu->SelectIndex(Index);

	ParentMenu->UpdateSelectedServerColor();
}

void UServerRow::OnHovered()
{
	if (Index != ParentMenu->GetSelectedIndex())
	{
		UpdateTextColor(ServerName, FLinearColor::Yellow);
		UpdateTextColor(NumPlayers, FLinearColor::Yellow);
	}
}

void UServerRow::OnUnhovored()
{
	if (Index != ParentMenu->GetSelectedIndex())
	{
		SetTextDefaultColor(ServerName);
		SetTextDefaultColor(NumPlayers);
	}
}

void UServerRow::UpdateTextColor(UTextBlock* TextBlock, const FLinearColor& Color)
{
	TextBlock->SetColorAndOpacity(FSlateColor(Color));
}

void UServerRow::SetTextDefaultColor(UTextBlock* TextBlock)
{
	TextBlock->SetColorAndOpacity(FSlateColor(DefaultTextColor));
}
