// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SFSaveGameData.generated.h"

USTRUCT(BlueprintType)
struct FShowdownModeData
{
	GENERATED_BODY()
public:
	void Initialize();
	void IncreaseShowdownCounter();
	void IncreaseSlowMotionTime();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int ShowdownCounter = 1;

	UPROPERTY()
	float SlowMotionTime = 0.1f;
};

class ASamuraiTrainerGameMode;

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API USFSaveGameData : public USaveGame
{
	GENERATED_BODY()
	
public:
	USFSaveGameData();
	void IncrementShowdownValues();

public:
	FORCEINLINE void SetGameMode(ASamuraiTrainerGameMode* Value) { GameMode = Value; }

public:
	UPROPERTY(VisibleAnywhere, Category = "Save Data")
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = "Showdown Mode Data")
	FShowdownModeData ShowdownModeData;

protected:
	ASamuraiTrainerGameMode* GameMode;
};
