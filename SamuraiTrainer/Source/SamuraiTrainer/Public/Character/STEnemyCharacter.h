// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/STBaseCharacter.h"
#include "Structs/STStructsHolder.h"
#include "STEnemyCharacter.generated.h"

#define HR_COMBO_END1 FName("HRComboEnd1")

class UAnimMontage;
class UBehaviorTree;
class ASTEnemyAIController;
class USTEnemyAnimInstance;
class ASTPlayerCharacter;
struct FAttackAndCounterReactionData;

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API ASTEnemyCharacter : public ASTBaseCharacter
{
	GENERATED_BODY()

public:
	ASTEnemyCharacter();

	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	virtual void PossessedBy(AController* NewController) override;

public:
	void PlayHitReaction(FName SectionName);

	// Called in the AI Controller
	UFUNCTION(BlueprintCallable)
	void PlaySwordAttack();

	virtual void PlayAttackStagger(FName SectionName) override;
	void PlayNextStagger();

	UFUNCTION(BlueprintCallable)
	ASTPlayerCharacter* GetPlayerCharacter();

public:
	// Movmentments
	virtual void SwordAttack() override;

public:
	FORCEINLINE void SetNextHitReactionSectionName(FName Value) { NextHitReactionSectionName = Value; }

protected:
	virtual void BeginPlay() override;

	void SubscribeToAnimationEvents();

protected:
	// Animations Event Handlers
	virtual void HandleAttackAnimCompleted() override;
	virtual void HandleStaggerAnimCompleted() override;
	virtual void HandleHitReactsionAnimCompleted() override;

	virtual void OnCounterAttackFrameBegan() override;
	virtual void OnCounterAttackFrameEnded() override;

protected:
	// Animations
	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageCEHitReaction;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageSwordAttacks;

protected:
	//APawn* PlayerPawn;
	ASTPlayerCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, Category = "Enemy AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ASTEnemyAIController> EnemyAIController;

	TArray<FAttackAndCounterReactionData> SwordAttacks;

private:
	USTEnemyAnimInstance* EnemyAnimInstance;
	FName NextHitReactionSectionName;
	FName NextStaggerSectionName;
	
};
