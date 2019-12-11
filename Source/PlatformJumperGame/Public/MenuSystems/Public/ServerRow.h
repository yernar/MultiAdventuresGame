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
	const FString& GetServerText() const;

	void OnSelected();
	void OnUnselected();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = ServerRowProps)
		class UTextBlock* ServerName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = ServerRowProps)
		class UTextBlock* NumPlayers;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget), Category = ServerRowProps)
		class UButton* RowButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = MenuProps)
		class UMainMenu* ParentMenu;
	
	uint32 Index;	// Index of Currrent ServerRow in {MainMenu => ServerList}

	static const FLinearColor DefaultTextColor;

public:
	FORCEINLINE void SetServerName(UTextBlock* Server) { ServerName = Server; }
	FORCEINLINE class UTextBlock* GetServerName() const { return ServerName; }
	

	FORCEINLINE void Setup(UMainMenu* Parent, uint32 Index)
	{
		ParentMenu = Parent;
		this->Index = Index;
		SetServerText("Server " + FString::FromInt(Index));
	}

	FORCEINLINE uint32 GetIndex() { return Index; }

private:
	UFUNCTION(BlueprintCallable)
		void OnClicked();
	UFUNCTION(BlueprintCallable)
		void OnHovered();
	UFUNCTION(BlueprintCallable)
		void OnUnhovored();

	void UpdateTextColor(class UTextBlock* TextBlock, const FLinearColor& Color);
	void SetTextDefaultColor(class UTextBlock* TextBlock);
};
