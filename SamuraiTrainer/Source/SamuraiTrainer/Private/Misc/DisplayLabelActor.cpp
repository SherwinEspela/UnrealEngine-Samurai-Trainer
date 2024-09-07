// Copyright 2024 Sherwin Espela. All rights reserved.


#include "Misc/DisplayLabelActor.h"
#include "Components/TextRenderComponent.h"

ADisplayLabelActor::ADisplayLabelActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADisplayLabelActor::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("DisplayLabel"));
}

void ADisplayLabelActor::DisplayLabels(bool ShouldDisplay)
{
	for (auto label : Labels)
	{
		label->SetActorHiddenInGame(ShouldDisplay);
	}
}
