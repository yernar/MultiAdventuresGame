// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMJUMPERGAME_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AMovingPlatform();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void AddActiveTrigger();
	void RemoveActiveTrigger();
	
	FORCEINLINE bool IsPlatformActive()
	{
		return bIsPlatformActive;
	}

	FORCEINLINE void SetPlatformActive(bool bIsActive = true)
	{
		bIsPlatformActive = bIsActive;
	}

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = MovingComponents)
		float MovementSpeed = 50.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = MovingComponents)
		FVector StartLocation;
	UPROPERTY()
		FVector EndLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true, MakeEditWidget = true), Category = MovingComponents)
		FVector TargetPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = TimelineComponents)
		class UTimelineComponent* PlatformMovement;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = TimelineComponents)
		class UCurveFloat* AlphaCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = TimelineComponents)
		float TimelinePlayRate = 4.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = TriggerComponents)
		bool bIsPlatformActive = true;
	

	UFUNCTION()
		void OnTimelineUpdate(float Alpha);

	UFUNCTION()
		void OnTimelineFinished();	
};
