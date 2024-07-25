// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "STBaseAnimInstance.generated.h"

class ASTBaseCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API USTBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float MovementSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float MovementOffsetYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	FRotator MovementRotation;

private:
	TObjectPtr<ASTBaseCharacter> BaseCharacter;
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
};
