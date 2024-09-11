// // Copyright 2024 Sherwin Espela. All rights reserved.


#include "Combat/CombatSystemAIController.h"

void ACombatSystemAIController::Initialize(TObjectPtr<UBehaviorTree> BehaviorTree)
{
	if (BehaviorTree == nullptr) return;
	RunBehaviorTree(BehaviorTree);
}
