// Copyright 2024 Sherwin Espela. All rights reserved.


#include "Character/MainMenu/SFMainMenuPlayerActor.h"

ASFMainMenuPlayerActor::ASFMainMenuPlayerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Display Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
}

void ASFMainMenuPlayerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

