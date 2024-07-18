// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/STBaseCharacter.h"
#include "STEnemyCharacter.generated.h"

#define HR_COMBO_END1 FName("HRComboEnd1")

class UAnimMontage;
class UAnimInstance;

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API ASTEnemyCharacter : public ASTBaseCharacter
{
	GENERATED_BODY()

public:
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

public:
	void PlayHitReaction(FName SectionName);
	
	UFUNCTION(BlueprintCallable)
	APawn* GetPlayerPawn();

	UFUNCTION(BlueprintCallable)
	void UpdateWarpTarget(APawn* Target);

	UFUNCTION(BlueprintCallable)
	FTransform GetAttackTransform() const;

public:
	FORCEINLINE void SetNextHitReactionSectionName(FName Value) { NextHitReactionSectionName = Value; }

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageHitReaction;

	APawn* PlayerPawn;

private:
	UAnimInstance* EnemyAnimInstance;
	FName NextHitReactionSectionName;
	
};
