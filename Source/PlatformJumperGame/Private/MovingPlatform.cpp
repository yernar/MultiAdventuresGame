// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "MovingPlatform.h"

#include "Components/TimelineComponent.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);

	// PlatformMovement = CreateDefaultSubobject<UTimelineComponent>(FName("PlatformMovement"));
	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("CurveFloat'/Game/Platforms/Miscellaneous/FC_Alpha.FC_Alpha'"));
	check(Curve.Succeeded());
	AlphaCurve = Curve.Object;
	
	FOnTimelineFloat InterpFunction; // Delegate function to be binded with [TimelineFloatReturn] function.	
	FOnTimelineEventStatic TimelineFinished; // Delegate function to be binded with [OnTimelineFinished] function.

	if (AlphaCurve)
	{
		PlatformMovement = NewObject<UTimelineComponent>(this, "PlatformMovement");
		PlatformMovement->CreationMethod = EComponentCreationMethod::UserConstructionScript;

		this->BlueprintCreatedComponents.Add(PlatformMovement);

		PlatformMovement->SetPropertySetObject(this);
		PlatformMovement->SetDirectionPropertyName(FName("MovementDirection"));
		PlatformMovement->SetLooping(false);
		PlatformMovement->SetTimelineLength(4.f);
		PlatformMovement->SetTimelineLengthMode(TL_LastKeyFrame);
		PlatformMovement->SetPlaybackPosition(0.f, false);


		InterpFunction.BindUFunction(this, FName("TimelineFloatReturn"));
		TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));

		PlatformMovement->AddInterpFloat(AlphaCurve, InterpFunction, FName("Alpha"));
		PlatformMovement->SetTimelineFinishedFunc(TimelineFinished);
		PlatformMovement->RegisterComponent();
	}	
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
	
	StartLocation = GetActorLocation();

	if (PlatformMovement)
		PlatformMovement->Play();	
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	// AddActorWorldOffset((GetTransform().TransformPosition(TargetPoint) - GetActorLocation()).GetSafeNormal() * MovingSpeed * DeltaTime);
}

void AMovingPlatform::TimelineFloatReturn(float Value)
{
	SetActorLocation(FMath::Lerp(StartLocation, GetTransform().TransformPosition(TargetPoint), Value));
}

void AMovingPlatform::OnTimelineFinished()
{
	if (!PlatformMovement->GetPlaybackPosition())
		PlatformMovement->Play();
		
	else if (PlatformMovement->GetPlaybackPosition() == PlatformMovement->GetTimelineLength())
		PlatformMovement->Reverse();
}
