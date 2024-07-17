// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/STBaseCharacter.h"
#include "STEnemyCharacter.generated.h"

class UAnimMontage;
class UAnimInstance;
//class UPawnSensingComponent;

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

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageHitReaction;

	APawn* PlayerPawn;

private:
	UAnimInstance* EnemyAnimInstance;
	
};
