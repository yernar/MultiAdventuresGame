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

void UServerRow::OnClicked()
{
	ParentMenu->SelectIndex(Index);
}
