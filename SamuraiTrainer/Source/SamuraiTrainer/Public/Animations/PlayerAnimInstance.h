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

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ASTPlayerCharacter* GetPlayer() const { return PlayerCharacter; }

protected:
	UFUNCTION(BlueprintCallable)
	void OnSwordInteractionCompleted();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Weapon State")
	EWeaponStates WeaponState;

private:
	ASTPlayerCharacter* PlayerCharacter;
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
	
};
