#pragma once

UENUM(BlueprintType)
enum class EPlayerStates : uint8
{
	EPS_IdleNoWeapon	UMETA(DisplayName = "Idle No Weapon"),
	EPS_IdleWithWeapon	UMETA(DisplayName = "Idle With Weapon"),
	EPS_Default			UMETA(DisplayName = "Default")
};

UENUM(BlueprintType)
enum class EMovementStates : uint8
{
	EPMS_Attacking		UMETA(DisplayName = "Attacking"),
	EPMS_Interacting	UMETA(DisplayName = "Interacting"),
	EPMS_Default		UMETA(DisplayName = "Default")
};

UENUM(BlueprintType)
enum class EWeaponStates : uint8
{
	EWS_Stored			UMETA(DisplayName = "Stored"),
	EWS_Holding			UMETA(DisplayName = "Holding"),
	EWS_Default			UMETA(DisplayName = "Default")
};
