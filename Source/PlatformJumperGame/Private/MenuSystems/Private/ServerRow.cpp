// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "ServerRow.h"
#include "MainMenu.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

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
	ServerName->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
	NumPlayers->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
}

void UServerRow::OnUnselected()
{
	OnUnhovored();
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
		ServerName->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));
		NumPlayers->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));
	}
}

void UServerRow::OnUnhovored()
{
	if (Index != ParentMenu->GetSelectedIndex())
	{
		ServerName->SetColorAndOpacity(FSlateColor(FLinearColor::White));
		NumPlayers->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	}
}
