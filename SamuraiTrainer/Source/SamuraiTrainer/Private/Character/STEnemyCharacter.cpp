// // Copyright 2024 Sherwin Espela. All rights reserved.


#include "Character/STEnemyCharacter.h"
#include "Character/STPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AI/STEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animations/STEnemyAnimInstance.h"
#include "ConstantValues.h"
#include "Engine/DamageEvents.h"

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

	EnemyAnimInstance = CastChecked<USTEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	SubscribeToAnimationEvents();
	
	GetCharacterMovement()->MaxWalkSpeed = 200.f;

	EnemyAIController = Cast<ASTEnemyAIController>(EnemyAnimInstance->TryGetPawnOwner()->GetController());
	if (EnemyAIController != nullptr)
	{
		EnemyAIController->Initialize(BehaviorTree);
	}

	PlayerCharacter = CastChecked<ASTPlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	FAttackAndCounterReactionData AttackData1;
	AttackData1.Attack = ATTACK_DOWNSLASH;
	AttackData1.CounterBlock = BLOCK_DOWNSLASH;
	AttackData1.CBStagger = STAGGER_DOWNSLASH;
	AttackData1.MWPSocketName = ATTACK_SOCKET_FRONT;

	FAttackAndCounterReactionData AttackData2;
	AttackData2.Attack = ATTACK_UPSLASH;
	AttackData2.CounterBlock = BLOCK_UPSLASH;
	AttackData2.CBStagger = STAGGER_UPSLASH;
	AttackData2.MWPSocketName = ATTACK_SOCKET_FRONT;

	SwordAttacks.Add(AttackData1);
	SwordAttacks.Add(AttackData2);
}

void ASTEnemyCharacter::SubscribeToAnimationEvents()
{
	if (EnemyAnimInstance == nullptr) return;

	EnemyAnimInstance->OnAttackAnimCompleted.AddDynamic(this, &ASTEnemyCharacter::HandleAttackAnimCompleted);
	EnemyAnimInstance->OnStaggeredAnimCompleted.AddDynamic(this, &ASTEnemyCharacter::HandleStaggerAnimCompleted);
	EnemyAnimInstance->OnHitReactionAnimCompleted.AddDynamic(this, &ASTEnemyCharacter::HandleHitReactsionAnimCompleted);
}

void ASTEnemyCharacter::HandleAttackAnimCompleted()
{
	Super::HandleAttackAnimCompleted();
}

void ASTEnemyCharacter::HandleStaggerAnimCompleted()
{
	Super::HandleStaggerAnimCompleted();
	if (EnemyAIController)
	{
		EnemyAIController->SetStaggered(false);
	}
}

void ASTEnemyCharacter::HandleHitReactsionAnimCompleted()
{
	Super::HandleHitReactsionAnimCompleted();
	if (EnemyAIController)
	{
		EnemyAIController->SetHitReacting(false);
	}
}

void ASTEnemyCharacter::OnCounterAttackFrameBegan()
{
	Super::OnCounterAttackFrameBegan();
	SetSlowMotion();
	PlayerCharacter->SetCanCounterAttack(true);
}

void ASTEnemyCharacter::OnCounterAttackFrameEnded()
{
	Super::OnCounterAttackFrameEnded();
	
	SetSlowMotion(false);
	PlayerCharacter->SetCanCounterAttack(false);
	if (!PlayerCharacter->DidCounterAttack()) {
		// Player failed to counter-attack
		FDamageEvent DamageEvent;
		PlayerCharacter->TakeDamage(SWORD_DAMAGE_ENEMY, DamageEvent, GetController(), this);
	}
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
			if (EnemyAIController)
			{
				EnemyAIController->SetHitReacting();
			}
			
			EnemyAnimInstance->Montage_Play(MontageHitReaction);
			EnemyAnimInstance->Montage_JumpToSection(NextHitReactionSectionName, MontageHitReaction);
		}
	}

	if (bCanApplyDamage)
	{
		Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	
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
	SwordAttack();
}

void ASTEnemyCharacter::PlayAttackStagger(FName SectionName)
{
	Super::PlayAttackStagger(SectionName);

	if (EnemyAnimInstance && EnemyAIController)
	{
		EnemyAIController->SetStaggered();
		EnemyAnimInstance->Montage_Play(MontageAttackStagger);
		EnemyAnimInstance->Montage_JumpToSection(SectionName, MontageAttackStagger);
	}
}

void ASTEnemyCharacter::PlayNextStagger()
{
	MovementState = EMovementStates::EPMS_Staggered;
	if (EnemyAnimInstance)
	{
		EnemyAnimInstance->Montage_Play(MontageAttackStagger);
		EnemyAnimInstance->Montage_JumpToSection(NextStaggerSectionName, MontageAttackStagger);
	}
}

ASTPlayerCharacter* ASTEnemyCharacter::GetPlayerCharacter()
{
	return PlayerCharacter;
}

void ASTEnemyCharacter::SwordAttack()
{
	if (EnemyAnimInstance == nullptr || MontageSwordAttacks == nullptr) return;

	int MaxIndex = SwordAttacks.Num();
	FAttackAndCounterReactionData AttackData = SwordAttacks[FMath::RandRange(0, MaxIndex - 1)];
	OnAttackStartedWith3Params.Broadcast(AttackData.CounterBlock, AttackData.HitReaction, EPlayerQTEResponseType::EPQTER_Block);
	CurrentMWPSocketName = AttackData.MWPSocketName;
	NextStaggerSectionName = AttackData.CBStagger;
	OnWarpTargetUpdated();
	EnemyAnimInstance->Montage_Play(MontageSwordAttacks);
	EnemyAnimInstance->Montage_JumpToSection(AttackData.Attack, MontageSwordAttacks);

	Super::SwordAttack();
}
