// Copyright 2024 Sherwin Espela. All rights reserved.
#pragma once

UENUM(BlueprintType)
enum class EPlayerQTEResponseType : uint8
{
	EPQTER_SwordAttack	UMETA(DisplayName = "Sword Attack"),
	EPQTER_Kick			UMETA(DisplayName = "Kick"),
	EPQTER_Block		UMETA(DisplayName = "Block"),
	EPQTER_Evade		UMETA(DisplayName = "Evade"),
	EPQTER_Default		UMETA(DisplayName = "Default")
};
