// // Copyright 2024 Sherwin Espela. All rights reserved.


#include "AI/STEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "ConstantBBKeys.h"
#include "Navigation/CrowdFollowingComponent.h"

ASTEnemyAIController::ASTEnemyAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void ASTEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ASTEnemyAIController::Initialize(TObjectPtr<UBehaviorTree> BehaviorTree)
{
	if (BehaviorTree == nullptr) return;
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	RunBehaviorTree(BehaviorTree);
	GetBlackboardComponent()->SetValueAsObject(BB_KEY_PLAYER_PAWN, PlayerPawn);
	SetIdle();

	UCrowdFollowingComponent* CrowdFollowingComponent = FindComponentByClass<UCrowdFollowingComponent>();
	if (CrowdFollowingComponent)
	{
		CrowdFollowingComponent->SetCrowdSeparation(CrowdSeparationEnabled);
		CrowdFollowingComponent->SetCrowdSeparationWeight(CrowdSeparationWeight);
		CrowdFollowingComponent->SetCrowdAvoidanceRangeMultiplier(CrowdAvoidanceRangeMultiplier);
		CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);
	}
}

void ASTEnemyAIController::SetAttacking(bool Value)
{
	ResetAllValues();
	GetBlackboardComponent()->SetValueAsBool(BB_KEY_ATTACKING, Value);
}

void ASTEnemyAIController::SetIdle(bool Value)
{
	ResetAllValues();
	GetBlackboardComponent()->SetValueAsBool(BB_KEY_IDLE, Value);
}

void ASTEnemyAIController::SetHitReacting(bool HitReacting)
{
	ResetAllValues();
	GetBlackboardComponent()->SetValueAsBool(BB_KEY_HIT_REACTING, HitReacting);
}

void ASTEnemyAIController::SetStaggered(bool Staggered)
{
	ResetAllValues();
	GetBlackboardComponent()->SetValueAsBool(BB_KEY_STAGGERED, Staggered);
}

void ASTEnemyAIController::SetBlocking(bool Value)
{
	ResetAllValues();
	GetBlackboardComponent()->SetValueAsBool(BB_KEY_BLOCKING, Value);
}

void ASTEnemyAIController::SetRecovering(bool Value)
{
	ResetAllValues();
	GetBlackboardComponent()->SetValueAsBool(BB_KEY_RECOVERING, Value);
}

void ASTEnemyAIController::SetDying(bool Value)
{
	ResetAllValues();
	GetBlackboardComponent()->SetValueAsBool(BB_KEY_DYING, Value);
}

void ASTEnemyAIController::ResetAllValues()
{
	GetBlackboardComponent()->SetValueAsBool(BB_KEY_ATTACKING, false);
	GetBlackboardComponent()->SetValueAsBool(BB_KEY_IDLE, false);
	GetBlackboardComponent()->SetValueAsBool(BB_KEY_HIT_REACTING, false);
	GetBlackboardComponent()->SetValueAsBool(BB_KEY_STAGGERED, false);
	GetBlackboardComponent()->SetValueAsBool(BB_KEY_BLOCKING, false);
	GetBlackboardComponent()->SetValueAsBool(BB_KEY_RECOVERING, false);
}
