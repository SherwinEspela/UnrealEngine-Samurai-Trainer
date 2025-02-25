// Copyright 2025 Sherwin Espela. All rights reserved.

#include "Utility/RandomLevelLoader.h"
#include "Kismet/GameplayStatics.h"

RandomLevelLoader::RandomLevelLoader()
{
	MaxLevelCount = 7;
}

void RandomLevelLoader::LoadRandomLevel(const UObject* WorldContextObject)
{
	int RandNumber = FMath::RandRange(1, MaxLevelCount);
	CurrentLevelIndex = RandNumber;
	UGameplayStatics::OpenLevel(WorldContextObject, FName(FString::Printf(TEXT("LevelMap%i"), CurrentLevelIndex)));
}
