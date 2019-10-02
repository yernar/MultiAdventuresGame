// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "MovingPlatform.h"

#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);
	
	static ConstructorHelpers::FObjectFinder<UCurveFloat> FloatingCurve(TEXT("CurveFloat'/Game/Platforms/Miscellaneous/FC_Alpha.FC_Alpha'"));
	AlphaCurve = FloatingCurve.Object;	
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}	

	if (AlphaCurve)
	{	
		float TimelineLength = 4.f;

		FOnTimelineFloat InterpFunction{}; // Delegate function to be binded with [TimelineFloatReturn] function.
		FOnTimelineEvent TimelineFinished{}; // Delegate function to be binded with [OnTimelineFinished] function.

		InterpFunction.BindUFunction(this, FName("OnTimelineUpdate"));
		TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));

		PlatformMovement = NewObject<UTimelineComponent>(this, FName("PlatformMovement"));
		PlatformMovement->CreationMethod = EComponentCreationMethod::UserConstructionScript;

		this->BlueprintCreatedComponents.Add(PlatformMovement);

		PlatformMovement->SetPropertySetObject(this);
		PlatformMovement->SetLooping(false);
		PlatformMovement->SetTimelineLength(TimelineLength);
		PlatformMovement->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
		PlatformMovement->SetPlaybackPosition(0.f, false);
		PlatformMovement->SetPlayRate(MovingSpeed / 100 * TimelinePlayRate);

		PlatformMovement->AddInterpFloat(AlphaCurve, InterpFunction, FName("AlphaValue"));
		PlatformMovement->SetTimelineFinishedFunc(TimelineFinished);
		PlatformMovement->SetLooping(false);
		PlatformMovement->SetIgnoreTimeDilation(true);
		PlatformMovement->RegisterComponent();
		
		StartLocation = GetActorLocation();
		EndLocation = GetTransform().TransformPosition(TargetPoint);

		PlatformMovement->Play();
	}			
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMovingPlatform::OnTimelineUpdate(float AlphaValue)
{	
	SetActorLocation(FMath::Lerp(StartLocation, EndLocation, AlphaValue));
}

void AMovingPlatform::OnTimelineFinished()
{
	if (!PlatformMovement->GetPlaybackPosition())
		PlatformMovement->Play();

	else if (PlatformMovement->GetPlaybackPosition() == PlatformMovement->GetTimelineLength())
		PlatformMovement->Reverse();
}
