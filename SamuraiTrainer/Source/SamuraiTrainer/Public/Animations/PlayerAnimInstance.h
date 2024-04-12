// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

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
	UPROPERTY(BlueprintReadOnly)
	float MovementSpeed;

private:
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
};
