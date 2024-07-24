// // Copyright 2024 Sherwin Espela. All rights reserved.


#include "AI/STEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "ConstantBBKeys.h"

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
}
