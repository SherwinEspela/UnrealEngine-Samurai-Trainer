// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CustomEnums.h"
#include "Animations/STBaseAnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class ASTPlayerCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API UPlayerAnimInstance : public USTBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable)
	void SetRandomEmoteIdleType();

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ASTPlayerCharacter* GetPlayer() const { return PlayerCharacter; }
	FORCEINLINE void SetPlayerState(EPlayerStates Value) { PlayerState = Value; }

protected:
	UFUNCTION(BlueprintCallable)
	void OnSwordInteractionCompleted();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Weapon State")
	EWeaponStates WeaponState;

	UPROPERTY(BlueprintReadOnly, Category = "Player States")
	EPlayerStates PlayerState = EPlayerStates::EPS_Combat;

	UPROPERTY(BlueprintReadOnly, Category = "Player Emote Idle Type")
	EPlayerEmoteIdleTypes EmoteIdleType;

private:
	ASTPlayerCharacter* PlayerCharacter;
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
	
};
