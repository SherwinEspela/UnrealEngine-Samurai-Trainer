// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CombatSystemAIController.generated.h"

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API ACombatSystemAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	void Initialize(TObjectPtr<UBehaviorTree> BehaviorTree);
};
