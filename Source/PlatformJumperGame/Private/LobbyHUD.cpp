// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "LobbyHUD.h"
#include "MainPlayerState.h"

#include "Engine/Font.h"

ALobbyHUD::ALobbyHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UFont> FredokaFontObject(TEXT("/Game/MenuSystem/Fonts/FredokaOne-Regular_Font"));
	FredokaFont = (FredokaFontObject.Object ? FredokaFontObject.Object : nullptr);
}

void ALobbyHUD::DrawHUD()
{
	if (GetOwningPlayerController() && Cast<AMainPlayerState>(GetOwningPlayerController()->PlayerState))
	{
		APlayerController* APC = GetOwningPlayerController();
		AMainPlayerState* AMPS = Cast<AMainPlayerState>(APC->PlayerState);
		EPlayerReadinessStatus PlayerStatus = AMPS->PlayerReadinessStatus;
		FString Status = (PlayerStatus == EPlayerReadinessStatus::NOT_READY ? "Not Ready"
			: (PlayerStatus == EPlayerReadinessStatus::NOT_ENOUGH_PLAYERS ? "Not Enough Players"
				: "Ready"));

		DrawText("Press R to Toggle Your Status: ", FLinearColor::Blue, 10.f, 10.f, FredokaFont);
		DrawText(Status, FLinearColor::Green, 250.f, 10.f, FredokaFont);
	}
}