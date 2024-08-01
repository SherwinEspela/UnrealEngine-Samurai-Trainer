// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SamuraiTrainerGameMode.generated.h"

UCLASS(minimalapi)
class ASamuraiTrainerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASamuraiTrainerGameMode();

	FORCEINLINE float GetSlowMotionTime() const { return SlowMotionTime; }

protected:
	UPROPERTY(EditAnywhere, Category = "Time Controls")
	float SlowMotionTime = 0.45f;
};



