// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */
USTRUCT()
struct FServerProperty
{
	GENERATED_BODY()

	FString Name;
	FString HostedUsername;
	uint32 CurrentPlayers;
	uint32 MaxPlayers;	
};

UCLASS()
class PLATFORMJUMPERGAME_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

public:
	void SetServerText(const FString& Text = "Unknown");
	const FString& GetServerText() const;

	void SetHostingUserText(const FString& Text = "Unknown");
	const FString& GetHostingUserText() const;

	void SetNumPlayersText(const FString& Text = "Unknown");
	const FString& GetNumPlayersText() const;

	void OnSelected();
	void OnUnselected();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = ServerRowProps)
		class UTextBlock* ServerName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = ServerRowProps)
		UTextBlock* HostingUser;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = ServerRowProps)
		UTextBlock* NumPlayers;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = ServerRowProps)
		class UButton* RowButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = MenuProps)
		class UMainMenu* ParentMenu;
	
	uint32 Index;	// Index of Currrent ServerRow in {MainMenu => ServerList}

	static const FLinearColor DefaultTextColor;

public:
	FORCEINLINE void SetServerName(UTextBlock* Server) { ServerName = Server; }
	FORCEINLINE class UTextBlock* GetServerName() const { return ServerName; }
	FORCEINLINE UTextBlock* GetHostingUser() const { return HostingUser; }
	FORCEINLINE UTextBlock* GetNumPlayers() const { return NumPlayers; }
	

	FORCEINLINE void Setup(UMainMenu* Parent, uint32 Index)
	{
		ParentMenu = Parent;
		this->Index = Index;
	}

	FORCEINLINE uint32 GetIndex() { return Index; }

private:
	UFUNCTION(BlueprintCallable)
		void OnClicked();
	UFUNCTION(BlueprintCallable)
		void OnHovered();
	UFUNCTION(BlueprintCallable)
		void OnUnhovored();

	void UpdateTextColor(const FLinearColor& Color);
	void SetTextDefaultColor();
};
