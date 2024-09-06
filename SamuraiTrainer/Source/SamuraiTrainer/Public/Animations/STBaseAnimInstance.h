// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnumDeathPoseType.h"
#include "STBaseAnimInstance.generated.h"

class ASTBaseCharacter;
class UCharacterMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHitReactionAnimCompletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackAnimCompletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStaggeredAnimCompletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBlockAnimCompletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyCanBlockEventSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOpponentCanHitReactEventSignature);

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
	FORCEINLINE void SetDead(){ bIsDead = true; }
	FORCEINLINE void SetIsHealthCritical() { bIsHealthCritical = true; }
	FORCEINLINE void SetDeathPoseType(EDeathPoseTypes Value) { DeathPoseType = Value; }
	FORCEINLINE float GetMovementSpeed() const { return MovementSpeed; }

public:
	// MULTICAST DELEGATES
	FHitReactionAnimCompletedSignature OnHitReactionAnimCompleted;
	FAttackAnimCompletedSignature OnAttackAnimCompleted;
	FStaggeredAnimCompletedSignature OnStaggeredAnimCompleted;
	FBlockAnimCompletedSignature OnBlockAnimCompleted;
	FEnemyCanBlockEventSignature OnEnemyCanBlockEvent;
	FOpponentCanHitReactEventSignature OnOpponentCanHitReactEvent;

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

	UFUNCTION(BlueprintCallable)
	virtual void HandleOpponentCanHitReactEvent();

protected:
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float MovementSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float MovementOffsetYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	FRotator MovementRotation;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsDead = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsHealthCritical = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EDeathPoseTypes DeathPoseType;

private:
	TObjectPtr<ASTBaseCharacter> BaseCharacter;
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
};
