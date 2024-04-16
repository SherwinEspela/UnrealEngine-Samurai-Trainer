#pragma once

UENUM(BlueprintType)
enum class EPlayerStates : uint8
{
	EPS_IdleNoWeapon	UMETA(DisplayName = "Idle No Weapon"),
	EPS_IdleWithWeapon	UMETA(DisplayName = "Idle With Weapon"),
	EPS_Default			UMETA(DisplayName = "Default")
};
