// Copyright 2024 Sherwin Espela. All rights reserved.


#include "DataPersistence/SFSaveGameData.h"
#include "Kismet/GameplayStatics.h"

USFSaveGameData::USFSaveGameData()
{
	SaveSlotName = TEXT("TestSaveSlot");
	UserIndex = 0;
}
