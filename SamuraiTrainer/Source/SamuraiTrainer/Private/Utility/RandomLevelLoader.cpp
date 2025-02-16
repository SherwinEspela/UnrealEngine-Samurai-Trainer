// Copyright 2024 Sherwin Espela. All rights reserved.


#include "Utility/RandomLevelLoader.h"
#include "Kismet/GameplayStatics.h"

RandomLevelLoader::RandomLevelLoader()
{
	MaxLevelCount = 2;
}

void RandomLevelLoader::LoadRandomLevel(const UObject* WorldContextObject)
{
	const int RandNumber = FMath::RandRange(1, MaxLevelCount);
	UGameplayStatics::OpenLevel(WorldContextObject, FName(FString::Printf(TEXT("LevelMap%i"), RandNumber)));
}

RandomLevelLoader::~RandomLevelLoader()
{
}


