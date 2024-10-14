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
#include "NiagaraComponent.h"
#include "SamuraiTrainer/SamuraiTrainerGameMode.h"
#include "Items/Katana.h"
#include "Kismet/KismetMathLibrary.h"

#define NV_LINEAR_COLOR FString("SpriteColor")
#define ATTACK_INDICATOR_COLOR_RED FLinearColor(FColor::Red)
#define ATTACK_INDICATOR_COLOR_YELLOW FLinearColor(FColor::Yellow)
#define ATTACK_INDICATOR_COLOR_BLUE FLinearColor(FColor::Blue)
#define ATTACK_INDICATOR_COLOR_GREEN FLinearColor(FColor::Green)

ASTEnemyCharacter::ASTEnemyCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	FXAttackIndicator = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FX Attack Indicator"));
	FXAttackIndicator->SetupAttachment(GetMesh());

	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	FXAttackIndicator->SetupAttachment(GetMesh(), FName("HAIR"));

	FXTargetIndicator = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FX Target Indicator"));
	FXTargetIndicator->SetupAttachment(GetMesh());
	FXTargetIndicator->SetupAttachment(GetMesh(), FName("SocketTargetIndicator"));
}

void ASTEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	EnemyAnimInstance = CastChecked<USTEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	SubscribeToAnimationEvents();
	
	GetCharacterMovement()->MaxWalkSpeed = 300.f;

	EnemyAIController = Cast<ASTEnemyAIController>(EnemyAnimInstance->TryGetPawnOwner()->GetController());
	if (EnemyAIController && !bDebugCannotMove)
	{
		EnemyAIController->Initialize(BehaviorTree);
	}

	PlayerCharacter = CastChecked<ASTPlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	CurrentTargetPawn = PlayerCharacter;

	FAttackAndCounterReactionData AttackData1;
	AttackData1.Attack = ATTACK_ENEMY_1;
	AttackData1.CounterBlock = BLOCK_DOWNSLASH;
	AttackData1.CBStagger = BLOCK_1;
	AttackData1.MWPSocketName = ATTACK_SOCKET_FRONT;
	AttackData1.ParryFatal = PARRY_FATAL_1;
	AttackData1.ParryFatalReaction = PARRY_FATAL_R_1;
	AttackData1.ParryHitReaction = PARRY_HR_1;
	AttackData1.DeathPoseType = EDeathPoseTypes::EDPT_DeathPoseParryFatal1;

	FAttackAndCounterReactionData AttackData2;
	AttackData2.Attack = ATTACK_ENEMY_2;
	AttackData2.CounterBlock = BLOCK_UPSLASH;
	AttackData2.CBStagger = STAGGER_UPSLASH;
	AttackData2.MWPSocketName = ATTACK_SOCKET_FRONT;
	AttackData2.ParryFatal = PARRY_FATAL_2;
	AttackData2.ParryFatalReaction = PARRY_FATAL_R_2;
	AttackData2.ParryHitReaction = PARRY_HR_2;
	AttackData2.DeathPoseType = EDeathPoseTypes::EDPT_DeathPoseParryFatal2;

	FAttackAndCounterReactionData AttackData3;
	AttackData3.Attack = ATTACK_ENEMY_3;
	AttackData3.CounterBlock = BLOCK_UPSLASH;
	AttackData3.CBStagger = BLOCK_2;
	AttackData3.MWPSocketName = ATTACK_SOCKET_FRONT;
	AttackData3.ParryFatal = PARRY_FATAL_2;
	AttackData3.ParryFatalReaction = PARRY_FATAL_R_2;
	AttackData3.ParryHitReaction = PARRY_HR_2;
	AttackData3.DeathPoseType = EDeathPoseTypes::EDPT_DeathPoseParryFatal2;

	//SwordAttacks.Add(AttackData1);
	SwordAttacks.Add(AttackData2);
	SwordAttacks.Add(AttackData3);

	BlockSectionNames.Add(BLOCK_1);
	BlockSectionNames.Add(BLOCK_2);
	BlockSectionNames.Add(BLOCK_3);
	BlockSectionNames.Add(BLOCK_4);

	HitReactionSectionNames.Add(HIT_REACTION_1);
	HitReactionSectionNames.Add(HIT_REACTION_2);
	HitReactionSectionNames.Add(HIT_REACTION_3);

	StaggerSectionNames.Add(BLOCK_1);
	StaggerSectionNames.Add(BLOCK_2);

	/*ParryHRSectionNames.Add(PARRY_HR_1);
	ParryHRSectionNames.Add(PARRY_HR_2);*/

	FXAttackIndicator->Deactivate();
	FXAttackIndicator->SetForceSolo(true);
	FXAttackIndicator->SetCustomTimeDilation(1.f/CurrentMode->GetSlowMotionTime());
	FXAttackIndicator->SetNiagaraVariableLinearColor(NV_LINEAR_COLOR, ATTACK_INDICATOR_COLOR_RED);
	FXAttackIndicator->OnSystemFinished.AddDynamic(this, &ASTEnemyCharacter::OnFXAttackIndicatorFinished);

	ShouldDisplayTargetIndicator(false);
}

void ASTEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerCharacter && !bDebugCannotMove && !bIsDead)
	{
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerCharacter->GetActorLocation());
		SetActorRotation(FRotator(0.f, LookAtRotation.Yaw, 0.f));
	}
}

void ASTEnemyCharacter::SubscribeToAnimationEvents()
{
	if (EnemyAnimInstance == nullptr) return;

	//EnemyAnimInstance->OnAttackAnimCompleted.AddDynamic(this, &ASTEnemyCharacter::HandleAttackAnimCompleted);
	//EnemyAnimInstance->OnStaggeredAnimCompleted.AddDynamic(this, &ASTEnemyCharacter::HandleStaggerAnimCompleted);
}

void ASTEnemyCharacter::HandleAttackAnimCompleted()
{
	Super::HandleAttackAnimCompleted();

	OnAttackCompleted.Broadcast();
	OnAttackCompletedFromThisEnemy.Broadcast(this);
}

void ASTEnemyCharacter::HandleStaggerAnimCompleted()
{
	Super::HandleStaggerAnimCompleted();
	EnemyAIController->SetStaggered(false);
	OnBlockCompleted.Broadcast();
	OnBlockCompletedFromThisEnemy.Broadcast(this);
}

void ASTEnemyCharacter::HandleHitReactsionAnimCompleted()
{
	Super::HandleHitReactsionAnimCompleted();
	
	if (bIsHealthCritical) {
		EnemyAnimInstance->SetIsHealthCritical();
	}
	else {
		EnemyAIController->SetRecovering();
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

void ASTEnemyCharacter::HandleDyingAnimationCompleted()
{
	Super::HandleDyingAnimationCompleted();
	DetachFromControllerPendingDestroy();
	EnemyAnimInstance->SetDead();
	OnDeathCompletedFromThisEnemy.Broadcast(this);
}

void ASTEnemyCharacter::HandleBloodSpillFXNotifyBegin()
{
	Super::HandleBloodSpillFXNotifyBegin();
	if (Katana) Katana->ShouldPlayBloodSpillFx();
}

void ASTEnemyCharacter::HandleBlockCompleted()
{
	EnemyAIController->SetRecovering();
	OnBlockCompleted.Broadcast();
	OnBlockCompletedFromThisEnemy.Broadcast(this);
}

void ASTEnemyCharacter::HandleParryHRAnimCompleted()
{
	ResetCounterAttackStates();
	EnemyAIController->SetRecovering();
}

void ASTEnemyCharacter::HandleParryBlockCompleted()
{
	OnBlockCompleted.Broadcast();
	OnBlockCompletedFromThisEnemy.Broadcast(this);
}

void ASTEnemyCharacter::OnFXAttackIndicatorFinished(UNiagaraComponent* Value)
{
	FXAttackIndicator->Deactivate();
}

EPlayerQTEResponseType ASTEnemyCharacter::GenerateRandomQTEResponse()
{
	EPlayerQTEResponseType ResponseType = EPlayerQTEResponseType::EPQTER_Block;

	int RandomNumber = FMath::RandRange(1, 3);
	
		switch (RandomNumber)
		{
		case 1:
			FXAttackIndicator->SetNiagaraVariableLinearColor(NV_LINEAR_COLOR, ATTACK_INDICATOR_COLOR_RED);
			ResponseType = EPlayerQTEResponseType::EPQTER_Counter;
			break;

		case 2:
			FXAttackIndicator->SetNiagaraVariableLinearColor(NV_LINEAR_COLOR, ATTACK_INDICATOR_COLOR_GREEN);
			ResponseType = EPlayerQTEResponseType::EPQTER_Kick;
			break;

		default:
			FXAttackIndicator->SetNiagaraVariableLinearColor(NV_LINEAR_COLOR, ATTACK_INDICATOR_COLOR_YELLOW);
			ResponseType = EPlayerQTEResponseType::EPQTER_Block;
			break;
		}

	return ResponseType;
}

float ASTEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead) return 0.f;

	if (bCanApplyDamage)
	{
		Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}

	if (EnemyAnimInstance && !bDebugCannotMove)
	{
		OnWarpTargetUpdated();

		if (bIsDead)
		{
			FXTargetIndicator->Deactivate();
			FXTargetIndicator->SetActive(false);

			if (IsKilledByParry)
			{
				EnemyAnimInstance->Montage_Play(MontageParryFatalReactions);
				EnemyAnimInstance->Montage_JumpToSection(CurrentAttackData.ParryFatalReaction, MontageParryFatalReactions);
				IsKilledByParry = false;
				SetDeathPoseType(CurrentAttackData.DeathPoseType);
			}
			else {
				EnemyAnimInstance->Montage_Play(MontageCEHitReaction);
				EnemyAnimInstance->Montage_JumpToSection(NextHitReactionSectionName, MontageCEHitReaction);
			}
		}
		else {
			EnemyAIController->SetHitReacting();
			const int RandomIndex = FMath::RandRange(0, HitReactionSectionNames.Num() - 1);
			const FName HRSectionName = HitReactionSectionNames[RandomIndex];
			EnemyAnimInstance->Montage_Play(MontageHitReaction);
			EnemyAnimInstance->Montage_JumpToSection(HRSectionName, MontageHitReaction);
		}
	}
	
	return 0.0f;
}

void ASTEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ASTEnemyCharacter::AttackReaction(float DamageAmount, FName HitReaction)
{
	const int BlockChances = FMath::RandRange(0, 100);
	bool CanBlock = BlockChances > 80;
	if (CanBlock)
	{

	}
	else {
		FDamageEvent DamageEvent;
		NextHitReactionSectionName = HitReaction;
		TakeDamage(DamageAmount, DamageEvent, PlayerCharacter->GetController(), PlayerCharacter);
	}
}

void ASTEnemyCharacter::PlayHitReaction(FName SectionName)
{
	if (EnemyAnimInstance && MontageHitReaction)
	{
		OnWarpTargetUpdated();
		EnemyAnimInstance->Montage_Play(MontageHitReaction);
		EnemyAnimInstance->Montage_JumpToSection(SectionName, MontageHitReaction);
	}
}

void ASTEnemyCharacter::PlaySwordAttack()
{
	if (bDebugCannotAttack) return;
	SwordAttack();
}

void ASTEnemyCharacter::PlayAttackStagger(FName SectionName)
{
	Super::PlayAttackStagger(SectionName);

	if (EnemyAnimInstance && EnemyAIController)
	{
		OnWarpTargetUpdated();
		EnemyAIController->SetStaggered();
		EnemyAnimInstance->Montage_Play(MontageAttackStagger);
		EnemyAnimInstance->Montage_JumpToSection(SectionName, MontageAttackStagger);
	}
}

// TODO: remove when no longer needed
void ASTEnemyCharacter::PlayNextStagger()
{
	MovementState = EMovementStates::EPMS_Staggered;
	if (EnemyAnimInstance)
	{
		int RandomIndex = FMath::RandRange(0, StaggerSectionNames.Num() - 1);
		FName StaggerSectionName = StaggerSectionNames[RandomIndex];
		EnemyAnimInstance->Montage_Play(MontageBlock);
		EnemyAnimInstance->Montage_JumpToSection(StaggerSectionName, MontageBlock);
	}
}

void ASTEnemyCharacter::PlayNextParryHitReaction()
{
	if (EnemyAnimInstance && MontageParryHitReactions)
	{
		EnemyAnimInstance->Montage_Play(MontageParryHitReactions);
		EnemyAnimInstance->Montage_JumpToSection(CurrentAttackData.ParryHitReaction, MontageParryHitReactions);
	}
}

void ASTEnemyCharacter::SetKilledByParry()
{
	IsKilledByParry = true;
}

void ASTEnemyCharacter::MakeNextDecision()
{
	int RandomValue = FMath::RandRange(0, 100);
	const bool ShouldAttack = RandomValue > 40;
	const bool ShouldCloseEvade = RandomValue > 10 && RandomValue <= 40;
	if (ShouldAttack)
	{
		EnemyAIController->SetAttacking();
	}
	else if (ShouldCloseEvade)
	{
		EnemyAIController->SetCloseEvading();
	}
	else {
		EnemyAIController->SetIdle();
	}
}

ASTPlayerCharacter* ASTEnemyCharacter::GetPlayerCharacter()
{
	return PlayerCharacter;
}

void ASTEnemyCharacter::SetDeathPoseType(EDeathPoseTypes Value)
{
	Super::SetDeathPoseType(Value);
	EnemyAnimInstance->SetDeathPoseType(Value);
}

void ASTEnemyCharacter::ShouldDisplayTargetIndicator(bool ShouldDisplay)
{
	if (FXTargetIndicator == nullptr) return;

	if (ShouldDisplay)
	{
		FXTargetIndicator->Activate();
	}
	else {
		FXTargetIndicator->Deactivate();
	}

	FXTargetIndicator->SetVisibility(ShouldDisplay);
}

void ASTEnemyCharacter::SwordAttack()
{
	if (EnemyAnimInstance == nullptr || MontageSwordAttacks == nullptr) return;

	PlaySoundVoiceAttack();
	MovementState = EMovementStates::EPMS_Attacking;
	int MaxIndex = SwordAttacks.Num();
	CurrentAttackData = SwordAttacks[FMath::RandRange(0, MaxIndex - 1)];
	EPlayerQTEResponseType ResponseType = GenerateRandomQTEResponse();
	OnAttackStartedWith3Params.Broadcast(CurrentAttackData.CounterBlock, CurrentAttackData.HitReaction, EPlayerQTEResponseType::EPQTER_Block);
	OnAttackBegan.Broadcast(EPlayerQTEResponseType::EPQTER_Block);
	OnAttackBeganFromThisEnemy.Broadcast(this, EPlayerQTEResponseType::EPQTER_Block);

	EHitDirectionType HitDirection = DetermineHitDirectionByLineTrace(GetActorLocation(), PlayerCharacter->GetActorLocation());
	PlayerCharacter->SetHitDirectionType(HitDirection);
	//OnAttackHitDirectionDetermined.Broadcast(HitDirection);

	CurrentMWPSocketName = CurrentAttackData.MWPSocketName;
	OnWarpTargetUpdated();
	EnemyAnimInstance->Montage_Play(MontageSwordAttacks);
	EnemyAnimInstance->Montage_JumpToSection(CurrentAttackData.Attack, MontageSwordAttacks);
	FXAttackIndicator->Activate();
}

void ASTEnemyCharacter::Block(FName SectionName)
{
	if (!MontageBlock) return;
	Super::Block();
	
	OnWarpTargetUpdated();
	EnemyAIController->SetBlocking();
	EnemyAnimInstance->Montage_Play(MontageBlock);
	EnemyAnimInstance->Montage_JumpToSection(SectionName, MontageBlock);
	OnBlockCompletedFromThisEnemy.Broadcast(this);
}

void ASTEnemyCharacter::Block()
{
	if (!MontageBlock) return;
	Super::Block();

	OnWarpTargetUpdated();
	EnemyAIController->SetBlocking();
	const int RandomIndex = FMath::RandRange(0, BlockSectionNames.Num() - 1);
	const FName BlockSectionName = BlockSectionNames[RandomIndex];
	EnemyAnimInstance->Montage_Play(MontageBlock);
	EnemyAnimInstance->Montage_JumpToSection(BlockSectionName, MontageBlock);
	OnBlockCompletedFromThisEnemy.Broadcast(this);
}

void ASTEnemyCharacter::Counter()
{
	if (!MontageCounter) return;
	PlaySoundVoiceAttack();
	EnemyAnimInstance->Montage_Play(MontageCounter);
	EnemyAnimInstance->Montage_JumpToSection(COUNTER5, MontageCounter);
}
