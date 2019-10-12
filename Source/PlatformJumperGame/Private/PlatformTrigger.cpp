// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.


#include "PlatformTrigger.h"
#include "MovingPlatform.h"

#include "Components/BoxComponent.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("TriggerVolume"));
	PressurePad = CreateDefaultSubobject<UStaticMeshComponent>(FName("PressurePad"));

	SetRootComponent(TriggerVolume);
	PressurePad->SetupAttachment(TriggerVolume);
}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnTriggerBeginOverlap);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnTriggerEndOverlap);
	// TriggerVolume->OnComponentEndOverlap.AddDynamic(this, FName("OnTriggerEndOverlap"));
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlatformTrigger::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	if (OverlappingActors.Num() == 1)
	{
		for (AMovingPlatform* Platform : PlatformsToTrigger)
			Platform->AddActiveTrigger();

		PressurePad->AddLocalOffset(FVector(0.f, 0.f, -10.f));
	}
}

void APlatformTrigger::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	if (OverlappingActors.Num() == 0)
	{
		for (AMovingPlatform* Platform : PlatformsToTrigger)
			Platform->RemoveActiveTrigger();

		PressurePad->AddLocalOffset(FVector(0.f, 0.f, 10.f));
	}
}

