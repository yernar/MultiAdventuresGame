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

protected:
	virtual bool Initialize() override;

public:
	void SetServerText(const FString& Text = "Unknown");

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = ServerProps)
		class UTextBlock* ServerName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = ServerProps)
		class UButton* RowButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = MenuProps)
		class UMainMenu* ParentMenu;
	
	uint32 Index;	// Index of Currrent ServerRow in {MainMenu => ServerList}

public:
	FORCEINLINE void SetServerName(UTextBlock* Server) { ServerName = Server; }
	FORCEINLINE class UTextBlock* GetServerName() { return ServerName; }

	FORCEINLINE void Setup(UMainMenu * Parent, uint32 Index)
	{
		ParentMenu = Parent;
		this->Index = Index;
	}

private:
	UFUNCTION(BlueprintCallable)
		void OnClicked();
};
