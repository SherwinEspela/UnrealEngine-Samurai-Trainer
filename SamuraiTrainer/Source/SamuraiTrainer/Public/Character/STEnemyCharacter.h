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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackCompletedFromThisEnemySignature, ASTEnemyCharacter*, Enemy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlockCompletedFromThisEnemySignature, ASTEnemyCharacter*, Enemy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathCompletedFromThisEnemySignature, ASTEnemyCharacter*, Enemy);

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API ASTEnemyCharacter : public ASTBaseCharacter
{
	GENERATED_BODY()

public:
	ASTEnemyCharacter();
	virtual void Tick(float DeltaTime) override;

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
	//void PlayNextParryHitReaction();
	void SetKilledByParry();
	void SetHitByParry();

	UFUNCTION(BlueprintCallable)
	void MakeNextDecision();

	UFUNCTION(BlueprintCallable)
	ASTPlayerCharacter* GetPlayerCharacter();

	virtual void SetDeathPoseType(EDeathPoseTypes Value) override;

	void ShouldDisplayTargetIndicator(bool ShouldDisplay = true);

public:
	// Movements
	virtual void SwordAttack() override;
	virtual void Block(FName SectionName) override;
	virtual void Block();
	virtual void Counter();

public:
	FORCEINLINE void SetNextHitReactionSectionName(FName Value) { NextHitReactionSectionName = Value; }
	FORCEINLINE TObjectPtr<ASTEnemyAIController> GetEnemyAIController() const { return EnemyAIController; }
	FORCEINLINE FName GetParryFatalSectionName() const { return CurrentAttackData.ParryFatal; }

public:
	// Multicast Delegates
	FOnAttackBeganFromThisEnemySignature OnAttackBeganFromThisEnemy;
	FOnAttackCompletedFromThisEnemySignature OnAttackCompletedFromThisEnemy;
	FOnBlockCompletedFromThisEnemySignature OnBlockCompletedFromThisEnemy;
	FOnDeathCompletedFromThisEnemySignature OnDeathCompletedFromThisEnemy;

protected:
	virtual void BeginPlay() override;
	void SubscribeToAnimationEvents();

public:
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

	UFUNCTION(BlueprintCallable)
	virtual void HandleParryHRAnimCompleted();

	UFUNCTION(BlueprintCallable)
	virtual void HandleParryBlockCompleted();

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

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageParryHitReactions;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageParryFatalReactions;

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
	//TArray<FName> ParryHRSectionNames;

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

	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraComponent* FXTargetIndicator;

private:
	USTEnemyAnimInstance* EnemyAnimInstance;
	FName NextHitReactionSectionName;
	FName NextStaggerSectionName;
	FAttackAndCounterReactionData CurrentAttackData;
	bool IsKilledByParry = false;
	bool IsHitByParry = false;
};
