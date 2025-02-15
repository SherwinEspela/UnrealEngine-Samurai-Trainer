// Copyright 2024 Sherwin Espela. All rights reserved.


#include "Combat/CombatSystemPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Character/STEnemyCharacter.h"
#include "Combat/CombatSystemAIController.h"
#include "AI/STEnemyAIController.h"
#include "Character/STPlayerCharacter.h"
#include "Combat/TargetLockActor.h"
#include "Components/CapsuleComponent.h"
#include "PlayerController/STPlayerController.h"

ACombatSystemPawn::ACombatSystemPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACombatSystemPawn::BeginPlay()
{
	Super::BeginPlay();

	Player = CastChecked<ASTPlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Player->OnAttackStarted.AddDynamic(this, &ACombatSystemPawn::HandlePlayerAttackStarted);
	Player->OnEnemiesCanAttack.AddDynamic(this, &ACombatSystemPawn::HandleEnemisCanAttack);
	Player->OnStaggerStarted.AddDynamic(this, &ACombatSystemPawn::HandlePlayerStaggerStarted);
	Player->OnCharacterDied.AddDynamic(this, &ACombatSystemPawn::HandlePlayerCharacterDied);
	
	PlayerController = CastChecked<ASTPlayerController>(Player->GetController());
	PlayerController->OnLevelIntroHandled.AddDynamic(this, &ACombatSystemPawn::HandleLevelIntroCompleted);

	bIsSequenceAttacking = true;

	TSubclassOf<ASTEnemyCharacter> EnemyClass = ASTEnemyCharacter::StaticClass();
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, EnemyClass, OutActors);

	if (OutActors.Num() > 0)
	{
		for (auto FoundActor : OutActors) {
			auto Enemy = Cast<ASTEnemyCharacter>(FoundActor);

			if (Enemy)
			{
				Enemy->OnAttackBeganFromThisEnemy.AddDynamic(this, &ACombatSystemPawn::HandleAttackBeganFromEnemy);
				Enemy->OnAttackCompletedFromThisEnemy.AddDynamic(this, &ACombatSystemPawn::HandleAttackCompletedFromEnemy);
				Enemy->OnBlockCompletedFromThisEnemy.AddDynamic(this, &ACombatSystemPawn::HandleBlockCompletedFromEnemy);
				Enemy->OnDeathCompletedFromThisEnemy.AddDynamic(this, &ACombatSystemPawn::HandleDeathCompletedFromEnemy);
				Enemy->OnOpponentWillBeDead.AddDynamic(this, &ACombatSystemPawn::HandleEnemyWillBeDead);

				Enemies.Add(Enemy);
				EnemiesQ.Enqueue(Enemy);
			}
		}
	}

	CombatSystemAIController = Cast<ACombatSystemAIController>(GetController());
}

void ACombatSystemPawn::HandlePlayerAttackStarted()
{
	SetEnemiesToPauseAttacking();
}

void ACombatSystemPawn::HandlePlayerStaggerStarted()
{
	SetEnemiesToPauseAttacking();
}

void ACombatSystemPawn::HandleEnemisCanAttack()
{
	SetEnemiesToPauseAttacking(false);
}

void ACombatSystemPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDebugging && CurrentEnemyAttacker)
	{
		DrawDebugSphere(GetWorld(), CurrentEnemyAttacker->GetActorLocation(), 70.f, 20.f, FColor::Green);
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
	for(auto AnEnemy : Enemies)
	{
		AnEnemy->ShouldDisplayTargetIndicator(false);
	}

	if (Player)
	{
		Player->SetCurrentAttackingEnemyWithResponseType(Enemy, PlayerResponseType);
	}

	SetEnemiesToPauseAttacking();
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
	try
	{
		Enemy->GetEnemyAIController()->SetChosenToAttack(false);
		Enemies.Remove(Enemy);

		SelectAttacker();
		//SetEnemiesToPauseAttacking(false);
	}
	catch (const std::exception&)
	{
		UE_LOG(LogTemp, Warning, TEXT("Exception handled........."));
	}
}

void ACombatSystemPawn::HandleEnemyAttackCompleted()
{
	bIsAttacking = false;
}

void ACombatSystemPawn::HandleEnemyBlockCompleted()
{
	bIsAttacking = false;
}

void ACombatSystemPawn::HandleEnemyWillBeDead()
{
	SetEnemiesToPauseAttacking();
}

void ACombatSystemPawn::SetEnemiesToPauseAttacking(bool Paused)
{
	for (auto AnEnemy : Enemies)
	{
	/*	if (!AnEnemy->IsDead() && !AnEnemy->IsAttacking())
		{
			AnEnemy->GetEnemyAIController()->SetPausedToAttack(Paused);
		}*/

		if (!AnEnemy->IsDead())
		{
			AnEnemy->GetEnemyAIController()->SetPausedToAttack(Paused);
		}
	}
}

void ACombatSystemPawn::HandleEventFromEnemyCompleted(ASTEnemyCharacter* Enemy)
{
	Enemy->GetEnemyAIController()->SetChosenToAttack(false);
	//SetEnemiesToPauseAttacking(false);
	EnemiesQ.Enqueue(Enemy);
}

void ACombatSystemPawn::SelectAttacker()
{
	try
	{
		if (Enemies.Num() <= 0) {
			CombatSystemAIController->StopBehavior();
			HandleAllEnemiesKilled();
			return;
		}

		ASTEnemyCharacter* NewAttacker;
		Player->RemoveCurrentAttackingEnemy();

		if (Enemies.Num() == 1)
		{
			NewAttacker = Enemies[0];
			NewAttacker->GetEnemyAIController()->SetChosenToAttack();
			return;
		}

		if (bIsSequenceAttacking)
		{
			if (!EnemiesQ.IsEmpty())
			{
				EnemiesQ.Dequeue(NewAttacker);
				NewAttacker->GetEnemyAIController()->SetChosenToAttack();
			}
		}
		else {
			if (bIsAttacking) return;

			if (CurrentEnemyAttacker)
			{
				CurrentEnemyAttacker->GetEnemyAIController()->SetChosenToAttack(false);
			}

			int RandomIndex = FMath::RandRange(0, Enemies.Num() - 1);
			NewAttacker = Enemies[RandomIndex];

			if (NewAttacker)
			{
				CurrentEnemyAttacker = NewAttacker;
				auto EnemyAIC = CurrentEnemyAttacker->GetEnemyAIController();
				if (CurrentEnemyAttacker && EnemyAIC)
				{
					EnemyAIC->SetChosenToAttack();
				}
			}
		}
	}
	catch (const std::exception&)
	{
		UE_LOG(LogTemp, Warning, TEXT("Exception handled........."));
	}
}

void ACombatSystemPawn::HandleAllEnemiesKilled()
{
	PlayerController->HandleAllEnemiesKilled();
}

void ACombatSystemPawn::HandleLevelIntroCompleted()
{
	CombatSystemAIController->Initialize(BehaviorTree);
}

void ACombatSystemPawn::HandlePlayerCharacterDied()
{
	SetPlayerDead();
	CombatSystemAIController->StopBehavior();
	SetEnemiesToPauseAttacking();
}

void ACombatSystemPawn::SetPlayerDead()
{
	for (auto AnEnemy : Enemies)
	{
		if (!AnEnemy->IsDead())
		{
			AnEnemy->GetEnemyAIController()->SetPlayerDead();
		}
	}
}
