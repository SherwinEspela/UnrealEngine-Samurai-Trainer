// Copyright 2024 Sherwin Espela. All rights reserved.


#include "Combat/TargetIndicatorActor.h"
#include "NiagaraComponent.h"

ATargetIndicatorActor::ATargetIndicatorActor()
{
	PrimaryActorTick.bCanEverTick = true;

	FXIndicator = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FX Indicator"));
	FXIndicator->SetupAttachment(GetRootComponent());
	FXIndicator->Deactivate();
}

void ATargetIndicatorActor::BeginPlay()
{
	Super::BeginPlay();
	FXIndicator->Deactivate();
}

void ATargetIndicatorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATargetIndicatorActor::DisplayIndicator(bool ShouldDisplay)
{
	if (ShouldDisplay)
	{
		FXIndicator->Activate();
	}
	else {
		FXIndicator->Deactivate();
	}
}
