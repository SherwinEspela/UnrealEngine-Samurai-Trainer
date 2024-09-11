// Copyright 2024 Sherwin Espela. All rights reserved.


#include "Combat/CombatSystemPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Character/STEnemyCharacter.h"
#include "Combat/CombatSystemAIController.h"
#include "AI/STEnemyAIController.h"
#include "Character/STPlayerCharacter.h"

ACombatSystemPawn::ACombatSystemPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACombatSystemPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDebugging && CurrentEnemyAttacker)
	{
		DrawDebugSphere(GetWorld(), CurrentEnemyAttacker->GetActorLocation(), 70.f, 20.f, FColor::Green);
	}
}

void ACombatSystemPawn::BeginPlay()
{
	Super::BeginPlay();

	Player = CastChecked<ASTPlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	TSubclassOf<ASTEnemyCharacter> EnemyClass = ASTEnemyCharacter::StaticClass();
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, EnemyClass, OutActors);

	if (OutActors.Num() > 0)
	{
		for (auto FoundActor : OutActors) {
			auto Enemy = Cast<ASTEnemyCharacter>(FoundActor);
			Enemies.Add(Enemy);
			EnemiesQ.Enqueue(Enemy);
		}
	}

	CombatSystemAIController = Cast<ACombatSystemAIController>(GetController());
	if (CombatSystemAIController)
	{
		CombatSystemAIController->Initialize(BehaviorTree);
	}
}

void ACombatSystemPawn::HandleEnemyAttackBegan(EPlayerQTEResponseType PlayerResponseType)
{
	if (CurrentEnemyAttacker && Player)
	{
		Player->SetCurrentAttackingEnemyWithResponseType(CurrentEnemyAttacker, PlayerResponseType);
		bIsAttacking = true;
	}
}

void ACombatSystemPawn::HandleAttackBeganFromEnemy(ASTEnemyCharacter* Enemy, EPlayerQTEResponseType PlayerResponseType)
{
	PrintPlayerQTEResponse(PlayerResponseType);

	if (Player)
	{
		Player->SetCurrentAttackingEnemyWithResponseType(Enemy, PlayerResponseType);
	}

	Enemy->GetEnemyAIController()->SetChosenToAttack(false);
	Enemy->OnAttackBeganFromThisEnemy.RemoveDynamic(this, &ACombatSystemPawn::HandleAttackBeganFromEnemy);
	EnemiesQ.Enqueue(Enemy);
}

void ACombatSystemPawn::HandleEnemyAttackCompleted()
{
	bIsAttacking = false;
	UE_LOG(LogTemp, Warning, TEXT("HandleEnemyAttackCompleted..."));
}

void ACombatSystemPawn::HandleEnemyBlockCompleted()
{
	bIsAttacking = false;
	UE_LOG(LogTemp, Warning, TEXT("HandleEnemyBlockCompleted..."));
}

void ACombatSystemPawn::SelectAttacker()
{
	if (Enemies.Num() <= 0) return;
	//if (bIsAttacking) return;

	ASTEnemyCharacter* NewAttacker;

	if (bIsSequenceAttacking)
	{
		if (!EnemiesQ.IsEmpty())
		{
			EnemiesQ.Dequeue(NewAttacker);
			NewAttacker->OnAttackBeganFromThisEnemy.AddDynamic(this, &ACombatSystemPawn::HandleAttackBeganFromEnemy);
		}
	}
	else {
		int RandomIndex = FMath::RandRange(0, Enemies.Num() - 1);
		NewAttacker = Enemies[RandomIndex];

		if (CurrentEnemyAttacker)
		{
			CurrentEnemyAttacker->GetEnemyAIController()->SetChosenToAttack(false);

			Player->RemoveCurrentAttackingEnemy();
			CurrentEnemyAttacker->OnAttackBegan.RemoveDynamic(this, &ACombatSystemPawn::HandleEnemyAttackBegan);
			CurrentEnemyAttacker->OnAttackCompleted.RemoveDynamic(this, &ACombatSystemPawn::HandleEnemyAttackCompleted);
			CurrentEnemyAttacker->OnBlockCompleted.RemoveDynamic(this, &ACombatSystemPawn::HandleEnemyBlockCompleted);

			/*for (auto Enemy : Enemies)
			{
				Enemy->GetEnemyAIController()->SetChosenToAttack(false);
			}*/
		}

		if (NewAttacker)
		{
			CurrentEnemyAttacker = NewAttacker;
			CurrentEnemyAttacker->GetEnemyAIController()->SetChosenToAttack();
			CurrentEnemyAttacker->OnAttackBegan.AddDynamic(this, &ACombatSystemPawn::HandleEnemyAttackBegan);
			CurrentEnemyAttacker->OnAttackCompleted.AddDynamic(this, &ACombatSystemPawn::HandleEnemyAttackCompleted);
			CurrentEnemyAttacker->OnBlockCompleted.AddDynamic(this, &ACombatSystemPawn::HandleEnemyBlockCompleted);
		}
	}
}
