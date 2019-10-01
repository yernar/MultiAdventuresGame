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

	

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = MovingComponents)
		float MovingSpeed = 50.f;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = MovingComponents)
		FVector StartLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true, MakeEditWidget = true), Category = MovingComponents)
		FVector TargetPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true, MakeEditWidget = true), Category = TimelineComponents)
		class UTimelineComponent* PlatformMovement;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true, MakeEditWidget = true), Category = TimelineComponents)
		class UCurveFloat* AlphaCurve;

	UFUNCTION()
		void TimelineFloatReturn(float Value);

	UFUNCTION()
		void OnTimelineFinished();

	
	
};
