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
	SetChosenToAttack(false);
	SetToOuterRange();
	//SetToMiddleRange(false);
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

void ASTEnemyAIController::SetChosenToAttack(bool Value)
{
	GetBlackboardComponent()->SetValueAsBool(BB_KEY_CHOSEN_TO_ATTACK, Value);

	bool IsHitReacting = GetBlackboardComponent()->GetValueAsBool(BB_KEY_HIT_REACTING);
	bool IsStaggered = GetBlackboardComponent()->GetValueAsBool(BB_KEY_STAGGERED);
	bool IsBlocking = GetBlackboardComponent()->GetValueAsBool(BB_KEY_BLOCKING);
	bool IsRecovering = GetBlackboardComponent()->GetValueAsBool(BB_KEY_RECOVERING);
	bool IsDying = GetBlackboardComponent()->GetValueAsBool(BB_KEY_DYING);
	if (!IsHitReacting && !IsStaggered && !IsBlocking && !IsRecovering && !IsDying && Value)
	{
		SetAttacking(true);
	}
}

void ASTEnemyAIController::SetPausedToAttack(bool Value)
{
	GetBlackboardComponent()->SetValueAsBool(BB_KEY_PAUSED_TO_ATTACK, Value);
}

void ASTEnemyAIController::SetToOuterRange(bool Value)
{
	GetBlackboardComponent()->SetValueAsBool(BB_KEY_AT_OUTER_RANGE, Value);
}

void ASTEnemyAIController::SetToMiddleRange(bool Value)
{
	GetBlackboardComponent()->SetValueAsBool(BB_KEY_AT_MIDDLE_RANGE, Value);
}

void ASTEnemyAIController::SetAttacking(bool Value)
{
	ResetAllValues();
	GetBlackboardComponent()->SetValueAsBool(BB_KEY_ATTACKING, Value);
}

void ASTEnemyAIController::SetCloseEvading(bool Value)
{
	ResetAllValues();
	GetBlackboardComponent()->SetValueAsBool(BB_KEY_CLOSE_EVADING, Value);
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
	GetBlackboardComponent()->SetValueAsBool(BB_KEY_CLOSE_EVADING, false);
}
