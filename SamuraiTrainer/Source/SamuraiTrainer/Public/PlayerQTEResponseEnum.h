// Copyright 2024 Sherwin Espela. All rights reserved.
#pragma once

UENUM(BlueprintType)
enum class EPlayerQTEResponseType : uint8
{
	EPQTER_SwordAttack	UMETA(DisplayName = "Sword Attack"),
	EPQTER_Kick			UMETA(DisplayName = "Kick"),
	EPQTER_Block		UMETA(DisplayName = "Block"),
	EPQTER_Counter		UMETA(DisplayName = "Counter"),
	EPQTER_Default		UMETA(DisplayName = "Default")
};

FORCEINLINE void PrintPlayerQTEResponse(EPlayerQTEResponseType PlayerQTEResponse) {
	UE_LOG(LogTemp, Log, TEXT("PlayerQTEResponse is : %s"), *UEnum::GetValueAsName(PlayerQTEResponse).ToString());
}