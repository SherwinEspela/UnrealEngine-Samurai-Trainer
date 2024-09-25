// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerQTEResponseEnum.h"
#include "EnumHitDirectionType.h"
#include "Containers/Queue.h"
#include "CombatSystemPawn.generated.h"

class ASTEnemyCharacter;
class UBehaviorTree;
class ACombatSystemAIController;
class ASTPlayerCharacter;
//class ATargetIndicatorActor;

UCLASS()
class SAMURAITRAINER_API ACombatSystemPawn : public APawn
{
	GENERATED_BODY()

public:
	ACombatSystemPawn();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SelectAttacker();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleEnemyAttackBegan(EPlayerQTEResponseType PlayerResponseType);

	UFUNCTION()
	void HandleAttackBeganFromEnemy(ASTEnemyCharacter* Enemy, EPlayerQTEResponseType PlayerResponseType);

	UFUNCTION()
	void HandleAttackCompletedFromEnemy(ASTEnemyCharacter* Enemy);

	UFUNCTION()
	void HandleBlockCompletedFromEnemy(ASTEnemyCharacter* Enemy);
	
	UFUNCTION()
	void HandleEnemyAttackCompleted();

	UFUNCTION()
	void HandleEnemyBlockCompleted();

	void SetEnemiesToPauseAttacking(bool Paused = true);

protected:
	TObjectPtr<ASTPlayerCharacter> Player;

	UPROPERTY(VisibleAnywhere, Category = "Enemies")
	TArray<ASTEnemyCharacter*> Enemies;

	TQueue<ASTEnemyCharacter*> EnemiesQ;

	UPROPERTY(EditAnywhere, Category = "Enemy AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	TObjectPtr<ACombatSystemAIController> CombatSystemAIController;

	UPROPERTY()
	TObjectPtr<ASTEnemyCharacter> CurrentEnemyAttacker;

	UPROPERTY(EditAnywhere, Category = "Debugging")
	bool bIsDebugging = true;

	bool bIsAttacking = false;
	bool bIsSequenceAttacking = true;

private:
	void HandleEventFromEnemyCompleted(ASTEnemyCharacter* Enemy);
};
