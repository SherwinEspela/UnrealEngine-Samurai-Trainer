// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "STEnemyAIController.generated.h"

class UBehaviorTreeComponent;
class UBehaviorTree;

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API ASTEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASTEnemyAIController();

	void Initialize(TObjectPtr<UBehaviorTree> BehaviorTree);
	void SetAttacking(bool Value = true);
	void SetIdle(bool Value = true);
	void SetHitReacting(bool HitReacting = true);
	void SetStaggered(bool Staggered = true);
	void SetBlocking(bool Value = true);
	void SetRecovering(bool Value = true);

protected:
	virtual void BeginPlay() override;

	void ResetAllValues();

protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	APawn* PlayerPawn;

};
