// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "ServerRow.h"
#include "Components/TextBlock.h"

void UServerRow::SetServerText(const FString& Text)
{
	GetServerName()->SetText(FText::FromString(Text));
}

