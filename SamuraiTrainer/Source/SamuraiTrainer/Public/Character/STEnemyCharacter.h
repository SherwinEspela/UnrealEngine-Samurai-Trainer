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
class UNiagaraComponent;
struct FAttackAndCounterReactionData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttackBeganFromThisEnemySignature, ASTEnemyCharacter*, Enemy,  EPlayerQTEResponseType, PlayerQTEResposeType);

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
	void AttackReaction(float Damage, FName HitReaction);

	void PlayHitReaction(FName SectionName);

	// Called in the AI Controller
	UFUNCTION(BlueprintCallable)
	void PlaySwordAttack();

	virtual void PlayAttackStagger(FName SectionName) override;
	void PlayNextStagger();

	UFUNCTION(BlueprintCallable)
	void MakeNextDecision();

	UFUNCTION(BlueprintCallable)
	ASTPlayerCharacter* GetPlayerCharacter();

	virtual void SetDeathPoseType(EDeathPoseTypes Value) override;

public:
	// Movmentments
	virtual void SwordAttack() override;
	virtual void Block(FName SectionName) override;
	virtual void Block();
	virtual void Counter();

public:
	FORCEINLINE void SetNextHitReactionSectionName(FName Value) { NextHitReactionSectionName = Value; }
	FORCEINLINE TObjectPtr<ASTEnemyAIController> GetEnemyAIController() const { return EnemyAIController; }

public:
	// Multicast Delegates
	FOnAttackBeganFromThisEnemySignature OnAttackBeganFromThisEnemy;

protected:
	virtual void BeginPlay() override;
	void SubscribeToAnimationEvents();

protected:
	// Animations Event Handlers
	virtual void HandleAttackAnimCompleted() override;
	virtual void HandleStaggerAnimCompleted() override;
	virtual void OnCounterAttackFrameBegan() override;
	virtual void OnCounterAttackFrameEnded() override;
	virtual void HandleDyingAnimationCompleted() override;
	virtual void HandleBloodSpillFXNotifyBegin() override;
	virtual void HandleHitReactsionAnimCompleted() override;

	UFUNCTION(BlueprintCallable)
	virtual void HandleBlockCompleted();

protected:
	// Niagara FX Event Handler
	UFUNCTION()
	void OnFXAttackIndicatorFinished(UNiagaraComponent* Value);

protected:
	EPlayerQTEResponseType GenerateRandomQTEResponse();

protected:
	// Animations
	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageCEHitReaction;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageSwordAttacks;

protected:
	ASTPlayerCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, Category = "Enemy AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ASTEnemyAIController> EnemyAIController;

	TArray<FAttackAndCounterReactionData> SwordAttacks;
	TArray<FName> BlockSectionNames;
	TArray<FName> HitReactionSectionNames;
	TArray<FName> StaggerSectionNames;

protected:
	// Debugging
	UPROPERTY(EditAnywhere, Category = "Debugging")
	bool bDebugCannotAttack = false;

	UPROPERTY(EditAnywhere, Category = "Debugging")
	bool bDebugCannotMove = false;

protected:
	// FX
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UNiagaraComponent* FXAttackIndicator;

private:
	USTEnemyAnimInstance* EnemyAnimInstance;
	FName NextHitReactionSectionName;
	FName NextStaggerSectionName;
	
};
