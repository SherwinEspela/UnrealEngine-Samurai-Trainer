// Copyright 2024 Sherwin Espela. All rights reserved.
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
	EPMS_Attacking			UMETA(DisplayName = "Attacking"),
	EPMS_PreAttacking		UMETA(DisplayName = "Pre Attacking"),
	EPMS_SwordAttacking		UMETA(DisplayName = "Sword Attacking"),
	EPMS_Interacting		UMETA(DisplayName = "Interacting"),
	EPMS_Idle				UMETA(DisplayName = "Idle"),
	EPMS_Blocking			UMETA(DisplayName = "Blocking"),
	EPMS_Staggered			UMETA(DisplayName = "Staggered"),
	EPMS_Walking			UMETA(DisplayName = "Walking"),
	EPMS_Moving				UMETA(DisplayName = "Moving"),
	EPMS_HitReacting		UMETA(DisplayName = "Hit Reacting"),
	EPMS_ComboEnding		UMETA(DisplayName = "Combo Ending"),
	EPMS_Parrying			UMETA(DisplayName = "Parrying"),
	EPMS_ParryAttacking		UMETA(DisplayName = "Parry Attacking"),
	EPMS_Death				UMETA(DisplayName = "Death"),
	EPMS_Default			UMETA(DisplayName = "Default")
};

UENUM(BlueprintType)
enum class EWeaponStates : uint8
{
	EWS_Stored			UMETA(DisplayName = "Stored"),
	EWS_Holding			UMETA(DisplayName = "Holding"),
	EWS_Default			UMETA(DisplayName = "Default")
};

UENUM(BlueprintType)
enum class EMainMenuButtonTypes : uint8
{
	EMMBT_Play			UMETA(DisplayName = "Play"),
	EMMBT_Modes			UMETA(DisplayName = "Modes"),
	EMMBT_Settings		UMETA(DisplayName = "Settings"),
	EMMBT_DevBio		UMETA(DisplayName = "DevBio"),
	EMMBT_Tutorials		UMETA(DisplayName = "Tutorials"),
	EMMBT_LevelResume	UMETA(DisplayName = "Level Resume"),
	EMMBT_LevelContinue	UMETA(DisplayName = "Level Continue"),
	EMMBT_LevelRestart	UMETA(DisplayName = "Level Restart"),
	EMMBT_LevelControls	UMETA(DisplayName = "Level Controls"),
	EMMBT_LevelSettings	UMETA(DisplayName = "Level Settings"),
	EMMBT_LevelExit		UMETA(DisplayName = "Level Exit"),
	EMMBT_Default		UMETA(DisplayName = "Default")
};

FORCEINLINE void PrintMovementState(EMovementStates MovementState) {
	UE_LOG(LogTemp, Log, TEXT("Movement State is : %s"), *UEnum::GetValueAsName(MovementState).ToString());
}

FORCEINLINE void PrintButtonType(EMainMenuButtonTypes ButtonType) {
	UE_LOG(LogTemp, Log, TEXT("Button Type is : %s"), *UEnum::GetValueAsName(ButtonType).ToString());
}
