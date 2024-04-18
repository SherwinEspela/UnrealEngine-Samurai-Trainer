// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CustomEnums.h"
#include "PlayerAnimInstance.generated.h"

class ASTPlayerCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintCallable)
	void OnSwordInteractionCompleted();

protected:
	UPROPERTY(BlueprintReadOnly)
	float MovementSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Player State")
	EPlayerStates WeaponState;

private:
	ASTPlayerCharacter* PlayerCharacter;
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
	
};
