// // Copyright 2024 Sherwin Espela. All rights reserved.


#include "Character/STEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AI/STEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ConstantValues.h"

ASTEnemyCharacter::ASTEnemyCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void ASTEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	EnemyAnimInstance = GetMesh()->GetAnimInstance();

	GetCharacterMovement()->MaxWalkSpeed = 200.f;

	EnemyAIController = Cast<ASTEnemyAIController>(EnemyAnimInstance->TryGetPawnOwner()->GetController());
	if (EnemyAIController != nullptr)
	{
		EnemyAIController->Initialize(BehaviorTree);
	}

	FEnemyAttackData AttackData1;
	AttackData1.Attack = ATTACK_DOWNSLASH;
	AttackData1.NextPlayerBlock = BLOCK_DOWNSLASH;
	AttackData1.NextStagger = STAGGER_DOWNSLASH;

	FEnemyAttackData AttackData2;
	AttackData2.Attack = ATTACK_UPSLASH;
	AttackData2.NextPlayerBlock = BLOCK_UPSLASH;
	AttackData2.NextStagger = STAGGER_UPSLASH;

	SwordAttackSectionNames.Add(AttackData1);
	SwordAttackSectionNames.Add(AttackData2);
}

float ASTEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (EnemyAnimInstance && MontageHitReaction)
	{
		if (bIsHealthCritical)
		{
			EnemyAnimInstance->Montage_Play(MontageCEHitReaction);
			EnemyAnimInstance->Montage_JumpToSection(NextHitReactionSectionName, MontageCEHitReaction);
		}
		else {
			EnemyAnimInstance->Montage_Play(MontageHitReaction);
			EnemyAnimInstance->Montage_JumpToSection(NextHitReactionSectionName, MontageHitReaction);
		}
	}

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	return 0.0f;
}

void ASTEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ASTEnemyCharacter::PlayHitReaction(FName SectionName)
{
	if (EnemyAnimInstance && MontageHitReaction)
	{
		EnemyAnimInstance->Montage_Play(MontageHitReaction);
		EnemyAnimInstance->Montage_JumpToSection(SectionName, MontageHitReaction);
	}
}

void ASTEnemyCharacter::PlaySwordAttack()
{
	//UE_LOG(LogTemp, Warning, TEXT("Play sword attack ......"));
	if (EnemyAnimInstance && MontageSwordAttacks)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Play sword attack ......"));
		int MaxIndex = SwordAttackSectionNames.Num();
		FEnemyAttackData AttackData = SwordAttackSectionNames[FMath::RandRange(0, MaxIndex - 1)];
		//OnAttackStarted.Broadcast(AttackData.NextPlayerBlock);
		OnAttackStartedWithTwoParams.Broadcast(AttackData.NextPlayerBlock, AttackData.NextStagger);
		//NextStaggerSectionName = AttackData.NextStagger;
		//UE_LOG(LogTemp, Warning, TEXT("NextStaggerSectionName: %s"), *AttackData.NextStagger.ToString());
		EnemyAnimInstance->Montage_Play(MontageSwordAttacks);
		EnemyAnimInstance->Montage_JumpToSection(AttackData.Attack, MontageSwordAttacks);
	}
}

void ASTEnemyCharacter::PlayAttackStagger(FName SectionName)
{
	Super::PlayAttackStagger(SectionName);

	if (EnemyAnimInstance)
	{
		EnemyAnimInstance->Montage_Play(MontageAttackStagger);
		EnemyAnimInstance->Montage_JumpToSection(SectionName, MontageAttackStagger);
	}
}

void ASTEnemyCharacter::PlayNextStagger()
{
	if (EnemyAnimInstance)
	{
		EnemyAnimInstance->Montage_Play(MontageAttackStagger);
		EnemyAnimInstance->Montage_JumpToSection(NextStaggerSectionName, MontageAttackStagger);
	}
}

APawn* ASTEnemyCharacter::GetPlayerPawn()
{
	return PlayerPawn;
}

void ASTEnemyCharacter::UpdateWarpTarget(APawn* Target)
{
	this->GetTransform();
	PlayerPawn = Target;
	OnWarpTargetUpdated();
}

FTransform ASTEnemyCharacter::GetAttackTransform(FName SocketName) const
{
	return this->GetMesh()->GetSocketTransform(SocketName, ERelativeTransformSpace::RTS_World);
}
