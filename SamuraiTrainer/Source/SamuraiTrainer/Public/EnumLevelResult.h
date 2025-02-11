// Copyright 2025 Sherwin Espela. All rights reserved.
#pragma once

UENUM(BlueprintType)
enum class ELevelResultType : uint8
{
	ELRT_Completed		UMETA(DisplayName = "Completed"),
	ELRT_PlayerDied		UMETA(DisplayName = "Player Died"),
	EDPT_Default		UMETA(DisplayName = "Default")
};
