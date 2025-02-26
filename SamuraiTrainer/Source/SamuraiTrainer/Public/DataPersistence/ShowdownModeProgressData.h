// Copyright 2025 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ShowdownModeProgressData.generated.h"

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API UShowdownModeProgressData : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Save Data")
	int ShowdownCounter = 1;
	
};
