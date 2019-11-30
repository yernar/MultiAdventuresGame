// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMJUMPERGAME_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetServerText(const FString& Text = "Unknown");

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = ServerProps)
		class UTextBlock* ServerName;

public:
	FORCEINLINE void SetServerName(UTextBlock* Server) { ServerName = Server; }
	FORCEINLINE UTextBlock* GetServerName() { return ServerName; }	
};
