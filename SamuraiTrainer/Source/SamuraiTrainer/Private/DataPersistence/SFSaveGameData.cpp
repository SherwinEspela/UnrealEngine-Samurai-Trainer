// Copyright 2024 Sherwin Espela. All rights reserved.


#include "DataPersistence/SFSaveGameData.h"
#include "Kismet/GameplayStatics.h"

#define SLOWMO_DEFAULT 0.1f;
#define SLOWMO_INCREMENT 0.03f;

USFSaveGameData::USFSaveGameData()
{
	SaveSlotName = TEXT("TestSaveSlot");
	UserIndex = 0;
}

void USFSaveGameData::IncrementShowdownValues()
{
	ShowdownModeData.IncreaseShowdownCounter();
	ShowdownModeData.IncreaseSlowMotionTime();
}

// Showdown Mode Data
void FShowdownModeData::Initialize()
{
	ShowdownCounter = 1;
	SlowMotionTime = SLOWMO_DEFAULT;
}

void FShowdownModeData::IncreaseShowdownCounter()
{
	ShowdownCounter += 1;
}

void FShowdownModeData::IncreaseSlowMotionTime()
{
	SlowMotionTime += SLOWMO_INCREMENT;
	if (SlowMotionTime > 1.0f) SlowMotionTime = 1.0f;
}
