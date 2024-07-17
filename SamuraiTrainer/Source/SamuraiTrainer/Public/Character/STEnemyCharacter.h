// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/STBaseCharacter.h"
#include "STEnemyCharacter.generated.h"

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
	void PlayHitReaction(FName SectionName);

	UFUNCTION(BlueprintCallable)
	APawn* GetPlayerPawn();

	UFUNCTION(BlueprintCallable)
	void UpdateWarpTarget(APawn* Target);

	UFUNCTION(BlueprintCallable)
	FTransform GetAttackTransform() const;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageHitReaction;

	APawn* PlayerPawn;

private:
	UAnimInstance* EnemyAnimInstance;
	
};
