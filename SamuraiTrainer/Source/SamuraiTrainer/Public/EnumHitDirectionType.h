// Copyright 2024 Sherwin Espela. All rights reserved.
#pragma once

UENUM(BlueprintType)
enum class EHitDirectionType : uint8
{
	EHDT_Front	UMETA(DisplayName = "Front"),
	EHDT_Back	UMETA(DisplayName = "Back"),
	EHDT_Left	UMETA(DisplayName = "Left"),
	EHDT_Right	UMETA(DisplayName = "Right")
};
