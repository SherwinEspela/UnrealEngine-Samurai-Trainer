// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "STBaseAnimInstance.generated.h"

class ASTBaseCharacter;
class UCharacterMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHitReactionAnimCompletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackAnimCompletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStaggeredAnimCompletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBlockAnimCompletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyCanBlockEventSignature);

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

public:
	// MULTICAST DELEGATES
	FHitReactionAnimCompletedSignature OnHitReactionAnimCompleted;
	FAttackAnimCompletedSignature OnAttackAnimCompleted;
	FStaggeredAnimCompletedSignature OnStaggeredAnimCompleted;
	FBlockAnimCompletedSignature OnBlockAnimCompleted;
	FEnemyCanBlockEventSignature OnEnemyCanBlockEvent;

protected:
	UFUNCTION(BlueprintCallable)
	virtual void HandleAttackCompleted();

	UFUNCTION(BlueprintCallable)
	virtual void HandleStaggerCompleted();

	UFUNCTION(BlueprintCallable)
	virtual void HandleHitReactionAnimCompleted();

	UFUNCTION(BlueprintCallable)
	virtual void HandleBlockAnimCompleted();

	UFUNCTION(BlueprintCallable)
	virtual void HandleEnemyCanBlockEvent();

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
