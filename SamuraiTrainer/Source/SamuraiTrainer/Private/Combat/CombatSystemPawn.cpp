// Copyright 2024 Sherwin Espela. All rights reserved.


#include "Combat/CombatSystemPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Character/STEnemyCharacter.h"
#include "Combat/CombatSystemAIController.h"
#include "AI/STEnemyAIController.h"
#include "Character/STPlayerCharacter.h"
#include "Combat/TargetLockActor.h"
#include "Components/CapsuleComponent.h"

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
	bIsSequenceAttacking = true;

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
	//PrintPlayerQTEResponse(PlayerResponseType);

	for(auto AnEnemy : Enemies)
	{
		AnEnemy->ShouldDisplayTargetIndicator(false);
	}

	if (Player)
	{
		Player->SetCurrentAttackingEnemyWithResponseType(Enemy, PlayerResponseType);
	}

	SetEnemiesToPauseAttacking();
	//Enemy->OnAttackBeganFromThisEnemy.RemoveDynamic(this, &ACombatSystemPawn::HandleAttackBeganFromEnemy);
}

void ACombatSystemPawn::HandleAttackCompletedFromEnemy(ASTEnemyCharacter* Enemy)
{
	HandleEventFromEnemyCompleted(Enemy);
}

void ACombatSystemPawn::HandleBlockCompletedFromEnemy(ASTEnemyCharacter* Enemy)
{
	Player->GetTargetLockActor()->SetLineEndVectors(
		Enemy->GetActorForwardVector() * -1, Enemy->GetActorRightVector() * 0
	);
	HandleEventFromEnemyCompleted(Enemy);
}

void ACombatSystemPawn::HandleDeathCompletedFromEnemy(ASTEnemyCharacter* Enemy)
{
	Enemy->GetEnemyAIController()->SetChosenToAttack(false);
	Enemy->OnAttackCompletedFromThisEnemy.RemoveDynamic(this, &ACombatSystemPawn::HandleAttackCompletedFromEnemy);
	Enemy->OnBlockCompletedFromThisEnemy.RemoveDynamic(this, &ACombatSystemPawn::HandleBlockCompletedFromEnemy);
	Enemy->OnDeathCompletedFromThisEnemy.RemoveDynamic(this, &ACombatSystemPawn::HandleDeathCompletedFromEnemy);
	Enemies.Remove(Enemy);

	SelectAttacker();
	SetEnemiesToPauseAttacking(false);
}

void ACombatSystemPawn::HandleEnemyAttackCompleted()
{
	bIsAttacking = false;
}

void ACombatSystemPawn::HandleEnemyBlockCompleted()
{
	bIsAttacking = false;
}

void ACombatSystemPawn::SetEnemiesToPauseAttacking(bool Paused)
{
	for (auto AnEnemy : Enemies)
	{
		if (!AnEnemy->IsAttacking())
		{
			AnEnemy->GetEnemyAIController()->SetPausedToAttack(Paused);
		}
	}
}

void ACombatSystemPawn::HandleEventFromEnemyCompleted(ASTEnemyCharacter* Enemy)
{
	Enemy->GetEnemyAIController()->SetChosenToAttack(false);
	Enemy->OnAttackCompletedFromThisEnemy.RemoveDynamic(this, &ACombatSystemPawn::HandleAttackCompletedFromEnemy);
	Enemy->OnBlockCompletedFromThisEnemy.RemoveDynamic(this, &ACombatSystemPawn::HandleBlockCompletedFromEnemy);
	SetEnemiesToPauseAttacking(false);
	EnemiesQ.Enqueue(Enemy);
}

void ACombatSystemPawn::SelectAttacker()
{
	if (Enemies.Num() <= 0) return;
	
	ASTEnemyCharacter* NewAttacker;
	Player->RemoveCurrentAttackingEnemy();

	if (Enemies.Num() == 1)
	{
		NewAttacker = Enemies[0];
		NewAttacker->GetEnemyAIController()->SetChosenToAttack();
		NewAttacker->OnAttackBegan.AddDynamic(this, &ACombatSystemPawn::HandleEnemyAttackBegan);
		NewAttacker->OnAttackCompleted.AddDynamic(this, &ACombatSystemPawn::HandleEnemyAttackCompleted);
		NewAttacker->OnBlockCompleted.AddDynamic(this, &ACombatSystemPawn::HandleEnemyBlockCompleted);
		return;
	}

	if (bIsSequenceAttacking)
	{
		if (!EnemiesQ.IsEmpty())
		{
			do
			{
				EnemiesQ.Dequeue(NewAttacker);
			} while (NewAttacker->IsDead());

			
			NewAttacker->GetEnemyAIController()->SetChosenToAttack();
			NewAttacker->OnAttackBeganFromThisEnemy.AddDynamic(this, &ACombatSystemPawn::HandleAttackBeganFromEnemy);
			NewAttacker->OnAttackCompletedFromThisEnemy.AddDynamic(this, &ACombatSystemPawn::HandleAttackCompletedFromEnemy);
			NewAttacker->OnBlockCompletedFromThisEnemy.AddDynamic(this, &ACombatSystemPawn::HandleBlockCompletedFromEnemy);
			NewAttacker->OnDeathCompletedFromThisEnemy.AddDynamic(this, &ACombatSystemPawn::HandleDeathCompletedFromEnemy);
		}
	}
	else {
		if (bIsAttacking) return;

		if (CurrentEnemyAttacker)
		{
			CurrentEnemyAttacker->GetEnemyAIController()->SetChosenToAttack(false);

			CurrentEnemyAttacker->OnAttackBegan.RemoveDynamic(this, &ACombatSystemPawn::HandleEnemyAttackBegan);
			CurrentEnemyAttacker->OnAttackCompleted.RemoveDynamic(this, &ACombatSystemPawn::HandleEnemyAttackCompleted);
			CurrentEnemyAttacker->OnBlockCompleted.RemoveDynamic(this, &ACombatSystemPawn::HandleEnemyBlockCompleted);
		}

		do
		{
			int RandomIndex = FMath::RandRange(0, Enemies.Num() - 1);
			NewAttacker = Enemies[RandomIndex];
		} while (NewAttacker->IsDead());

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
