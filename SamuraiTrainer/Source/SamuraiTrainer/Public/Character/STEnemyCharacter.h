// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/STBaseCharacter.h"
#include "STEnemyCharacter.generated.h"

#define HR_COMBO_END1 FName("HRComboEnd1")

class UAnimMontage;
class UAnimInstance;
class UBehaviorTree;
class ASTEnemyAIController;

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
	
	UFUNCTION(BlueprintCallable)
	APawn* GetPlayerPawn();

	UFUNCTION(BlueprintCallable)
	void UpdateWarpTarget(APawn* Target);

	UFUNCTION(BlueprintCallable)
	FTransform GetAttackTransform(FName SocketName) const;

public:
	FORCEINLINE void SetNextHitReactionSectionName(FName Value) { NextHitReactionSectionName = Value; }

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageHitReaction;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageCEHitReaction;

	APawn* PlayerPawn;

	UPROPERTY(EditAnywhere, Category = "Enemy AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ASTEnemyAIController> EnemyAIController;

private:
	UAnimInstance* EnemyAnimInstance;
	FName NextHitReactionSectionName;
	
};
