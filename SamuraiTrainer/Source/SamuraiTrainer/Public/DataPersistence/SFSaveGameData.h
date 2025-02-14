// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SFSaveGameData.generated.h"

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API USFSaveGameData : public USaveGame
{
	GENERATED_BODY()
	
public:
	USFSaveGameData();

public:
	UPROPERTY(VisibleAnywhere, Category = "Save Data")
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = "Save Data")
	int ShowdownCounter = 4;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;
};
