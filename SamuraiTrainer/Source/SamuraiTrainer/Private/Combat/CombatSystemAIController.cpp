// // Copyright 2024 Sherwin Espela. All rights reserved.


#include "Combat/CombatSystemAIController.h"
#include "BrainComponent.h"

void ACombatSystemAIController::Initialize(TObjectPtr<UBehaviorTree> BehaviorTree)
{
	if (BehaviorTree == nullptr) return;
	
	RunBehaviorTree(BehaviorTree);
}

void ACombatSystemAIController::StopBehavior()
{
	BrainComponent->Deactivate();
	BrainComponent->StopLogic("");
}
