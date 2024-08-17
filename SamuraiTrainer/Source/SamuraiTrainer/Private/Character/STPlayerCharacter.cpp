// Copyright 2024 Sherwin Espela. All rights reserved.


#include "Character/STPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Character/STEnemyCharacter.h"
#include "Animations/PlayerAnimInstance.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

ASTPlayerCharacter::ASTPlayerCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bInheritPitch = true;
	CameraBoom->bInheritYaw = true;
	CameraBoom->bInheritRoll = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = true;

	CapsuleEnemyDetector = CreateDefaultSubobject<UCapsuleComponent>(TEXT("FrontEnemyDetector"));
	CapsuleEnemyDetector->SetupAttachment(CameraBoom);
}

void ASTPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	bIsSwordArmed = false;
	WeaponState = EWeaponStates::EWS_Stored;
	bCanPerformNextAttack = true;

	if (CapsuleEnemyDetector)
	{
		CapsuleEnemyDetector->OnComponentBeginOverlap.AddDynamic(this, &ASTPlayerCharacter::OnEnemyDetectorBeginOverlap);
	}

	InitPlayerAnimInstance();
	InitQueues();
}

void ASTPlayerCharacter::InitPlayerAnimInstance()
{
	PlayerAnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (PlayerAnimInstance)
	{
		PlayerAnimInstance->OnAttackAnimCompleted.AddDynamic(this, &ASTPlayerCharacter::HandleAttackAnimCompleted);
		PlayerAnimInstance->OnBlockAnimCompleted.AddDynamic(this, &ASTPlayerCharacter::HandleBlockAnimCompleted);
		PlayerAnimInstance->OnHitReactionAnimCompleted.AddDynamic(this, &ASTPlayerCharacter::HandleHitReactsionAnimCompleted);
		PlayerAnimInstance->OnEnemyCanBlockEvent.AddDynamic(this, &ASTPlayerCharacter::HandleEnemyCanBlockEvent);
		PlayerAnimInstance->OnStaggeredAnimCompleted.AddDynamic(this, &ASTPlayerCharacter::HandleStaggerAnimCompleted);
	}
}

void ASTPlayerCharacter::InitQueues()
{
	FrontAttackQueues.Enqueue(FAttackData(ATTACK_DOWNSLASH, HIT_REACTION_DOWNSLASH, ATTACK_SOCKET_FRONT, BLOCK_DOWNSLASH, STAGGER_DOWNSLASH));
	FrontAttackQueues.Enqueue(FAttackData(ATTACK_UPSLASH, HIT_REACTION_UPSLASH, ATTACK_SOCKET_FRONT, BLOCK_UPSLASH, STAGGER_UPSLASH));

	BackAttackQueues.Enqueue(FAttackData(ATTACK_DOWNSLASH, HR_BEHIND1, ATTACK_SOCKET_BACK));
	BackAttackQueues.Enqueue(FAttackData(ATTACK_UPSLASH, HR_BEHIND2, ATTACK_SOCKET_BACK));

	KickQueues.Enqueue(FAttackData(KICK1, HIT_REACTION_KICK1, ATTACK_SOCKET_FRONT));
	KickQueues.Enqueue(FAttackData(KICK2, HIT_REACTION_KICK2, ATTACK_SOCKET_FRONT));

	SwordAttackComboEnders.Add(FAttackData(SWORD_ATTACK_COMBO_END1, HIT_REACTION_CE1, ATTACK_SOCKET_FRONT));
	SwordAttackComboEnders.Add(FAttackData(SWORD_ATTACK_COMBO_END2, HIT_REACTION_CE2, ATTACK_SOCKET_FRONT));
	BackComboEnders.Add(FAttackData(SA_BACK_CE1, HR_BACK_CE1, ATTACK_SOCKET_BACK));

	KickComboEnders.Add(FAttackData(KICK_COMBO_END1, HR_KICK_CE1));

	//AttackCounterList.Add(FAttackData(ATTACK_COUNTER1, HR_COUNTER1));
	CounterQueues.Enqueue(FAttackData(ATTACK_COUNTER1, HR_COUNTER1, ATTACK_SOCKET_FRONT));
	CounterQueues.Enqueue(FAttackData(ATTACK_COUNTER2, HR_COUNTER2, ATTACK_SOCKET_FRONT));
	CounterQueues.Enqueue(FAttackData(ATTACK_COUNTER3, HR_COUNTER_BLANK, ATTACK_SOCKET_FRONT));
	CounterQueues.Enqueue(FAttackData(ATTACK_COUNTER4, HR_COUNTER_BLANK, ATTACK_SOCKET_FRONT));
	CounterQueues.Enqueue(FAttackData(ATTACK_COUNTER5, HR_COUNTER_BLANK, ATTACK_SOCKET_FRONT));

	CounterComboEnders.Add(FAttackData(COUNTER_COMBO_END1, HR_COUNTER_CE1, ATTACK_SOCKET_FRONT));
}

void ASTPlayerCharacter::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	if (bIsInteractingWithWeapon) return;
	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
}

void ASTPlayerCharacter::SwordInteract()
{
	if (bIsInteractingWithWeapon) return;

	if (MontageSwordInteract)
	{
		bIsInteractingWithWeapon = true;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(MontageSwordInteract);
		FName SectionName = bIsSwordArmed ? STRING_SHEATHE : STRING_UNSHEATHE;
		AnimInstance->Montage_JumpToSection(SectionName, MontageSwordInteract);
		bIsSwordArmed = !bIsSwordArmed;
		WeaponState = bIsSwordArmed ? EWeaponStates::EWS_Holding : EWeaponStates::EWS_Stored;
	}
}

void ASTPlayerCharacter::SetNextAttackSectionName(FName Value)
{
	NextAttackSectionName = Value;
}

void ASTPlayerCharacter::SetNextHitReactionSectionName(FName Value)
{
	NextHitReactionSectionName = Value;
}

void ASTPlayerCharacter::EnemyInteract(
	TQueue<FAttackData> &MoveQueue, UAnimMontage* MontageToPlay, TArray<FAttackData> ComboEnders, 
	UAnimMontage* MontageEnder, float Damage, bool IsEnemyFacingFront
)
{
	if (!bCanPerformNextAttack) return;
	if (PlayerAnimInstance == nullptr) return;

	FAttackData AttackData;
	MoveQueue.Dequeue(AttackData);

	if (CurrentEnemy)
	{
		if (CurrentEnemy->IsAttacking() && bCanCounterAttack) bDidCounterAttack = true;

		if (CurrentEnemy->IsHealthCritical())
		{
			CurrentAttackData = ComboEnders[FMath::RandRange(0, ComboEnders.Num() - 1)];
			PlayerAnimInstance->Montage_Play(MontageEnder);
			PlayerAnimInstance->Montage_JumpToSection(CurrentAttackData.Attack, MontageEnder);
		}
		else {
			PlayerAnimInstance->Montage_Play(MontageToPlay);
			PlayerAnimInstance->Montage_JumpToSection(CurrentAttackData.Attack, MontageToPlay);
		}

		CurrentMWPSocketName = AttackData.AttackSocketName;
		OnWarpTargetUpdated();

		const int BlockChances = FMath::RandRange(0, 100);
		bEnemyCanBlock = BlockChances > 50;

		if (!bEnemyCanBlock)
		{
			// Enemy takes hit
			CurrentEnemy->SetNextHitReactionSectionName(CurrentAttackData.HitReaction);
			FDamageEvent DamageEvent;
			// TODO: remove hardcoded damage value
			CurrentEnemy->TakeDamage(Damage, DamageEvent, GetController(), this);
		}
	}
	else {
		PlayerAnimInstance->Montage_Play(MontageToPlay);
		PlayerAnimInstance->Montage_JumpToSection(AttackData.Attack, MontageToPlay);
	}

	MoveQueue.Enqueue(AttackData);
	MovementState = EMovementStates::EPMS_Attacking;
	bCanPerformNextAttack = false;
}

void ASTPlayerCharacter::SwordAttack()
{
	SetSlowMotion(false);
	if (bIsQTEMode)
	{
		CurrentPlayerQTEResponse = EPlayerQTEResponseType::EPQTER_SwordAttack;
		QTEResult();
		return;
	}

	ExecuteSwordAttack();
}

void ASTPlayerCharacter::Block()
{
	SetSlowMotion(false);
	if (bIsQTEMode)
	{
		CurrentPlayerQTEResponse = EPlayerQTEResponseType::EPQTER_Block;
		QTEResult();
		return;
	}

	ExecuteBlock();
}

void ASTPlayerCharacter::Kick()
{
	SetSlowMotion(false);
	if (bIsQTEMode)
	{
		CurrentPlayerQTEResponse = EPlayerQTEResponseType::EPQTER_Kick;
		QTEResult();
		return;
	}

	ExecuteKick();
}

void ASTPlayerCharacter::Counter()
{
	SetSlowMotion(false);
	if (bIsQTEMode)
	{
		CurrentPlayerQTEResponse = EPlayerQTEResponseType::EPQTER_Counter;
		QTEResult();
		return;
	}

	ExecuteCounter();
}

void ASTPlayerCharacter::HitReact()
{
	Super::HitReact();

	if (PlayerAnimInstance && MontageHitReaction)
	{
		PlayerAnimInstance->Montage_Play(MontageHitReaction);
		PlayerAnimInstance->Montage_JumpToSection(CurrentHRSectionName, MontageHitReaction);
	}
}

void ASTPlayerCharacter::PlayAttackStagger(FName SectionName)
{
	Super::PlayAttackStagger(SectionName);

	if (PlayerAnimInstance && MontageAttackStagger)
	{
		PlayerAnimInstance->Montage_Play(MontageAttackStagger);
		PlayerAnimInstance->Montage_JumpToSection(CurrentHRSectionName, MontageAttackStagger);
	}
}

void ASTPlayerCharacter::OnComboFrameBegan(bool IsLastBasicAttack)
{
	bIsLastBasicAttack = IsLastBasicAttack;
	bCanPerformNextAttack = true;
}

void ASTPlayerCharacter::OnComboFrameEnded()
{
	bCanPerformNextAttack = false;
}

void ASTPlayerCharacter::OnComboEnderStarted()
{
}

void ASTPlayerCharacter::OnComboEnderCompleted()
{
	HandleBasicAttackCompleted();
}

void ASTPlayerCharacter::HandleAttackAnimCompleted()
{
	Super::HandleAttackAnimCompleted();
	HandleBasicAttackCompleted();
}

void ASTPlayerCharacter::HandleEnemyCanBlockEvent()
{
	if (bEnemyCanBlock)
	{
		// Enemy blocks or evade the attack
		CurrentEnemy->Block(CurrentAttackData.Block);
		PlayerAnimInstance->Montage_Play(MontageAttackStagger);
		PlayerAnimInstance->Montage_JumpToSection(CurrentAttackData.AttackStagger, MontageAttackStagger);
	}
}

void ASTPlayerCharacter::HandleBasicAttackCompleted()
{
	bDidCounterAttack = false;
	bCanCounterAttack = false;
	bCanPerformNextAttack = true;
	bIsLastBasicAttack = false;
	NextAttackSectionName = ATTACK_DOWNSLASH;
	NextHitReactionSectionName = HIT_REACTION_DOWNSLASH;
	SetMovementState(EMovementStates::EPMS_Idle);
}

void ASTPlayerCharacter::OnCounterAttackFrameBegan()
{
	Super::OnCounterAttackFrameBegan();
}

ASTEnemyCharacter* ASTPlayerCharacter::GetTargetLockedEnemy() const
{
	return CurrentEnemy;
}

float ASTPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HitReact();
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	return 0.0f;
}

void ASTPlayerCharacter::OnEnemyDetectorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CurrentEnemy && OtherActor->GetName() == CurrentEnemy->GetName()) return;
	if (OtherActor) {
		CurrentEnemy = Cast<ASTEnemyCharacter>(OtherActor);
		if (CurrentEnemy)
		{
			CurrentEnemy->OnAttackStartedWith3Params.AddDynamic(this, &ASTPlayerCharacter::HandleOpponentAttackStarted);
			CurrentTargetPawn = CurrentEnemy;
		}
	}
}

void ASTPlayerCharacter::HandleOpponentAttackStarted(FName BlockSectionName, FName HRSectionName, EPlayerQTEResponseType ResponseType)
{
	CurrentBlockSectionName = BlockSectionName;
	CurrentHRSectionName = HRSectionName;
	ExpectedPlayerQTEResponse = ResponseType;
	bCanSwordAttack = false;
	bIsQTEMode = true;
}

void ASTPlayerCharacter::QTEResult()
{
	bIsQTEMode = false;
	bDidCounterAttack = true;

	if (ExpectedPlayerQTEResponse != CurrentPlayerQTEResponse)
	{
		// Player made a wrong QTE response
		FDamageEvent DamageEvent;
		TakeDamage(SWORD_DAMAGE_ENEMY, DamageEvent, GetController(), this);
		return;
	}

	switch (CurrentPlayerQTEResponse)
	{
	case EPlayerQTEResponseType::EPQTER_SwordAttack:
		ExecuteSwordAttack();
		break;
	case EPlayerQTEResponseType::EPQTER_Kick:
		ExecuteKick();
		break;
	case EPlayerQTEResponseType::EPQTER_Block:
		ExecuteBlock();
		break;
	case EPlayerQTEResponseType::EPQTER_Counter:
		ExecuteCounter();
		break;
	default:
		break;
	}
}

void ASTPlayerCharacter::ExecuteSwordAttack()
{
	//if (MovementState != EMovementStates::EPMS_Idle) return;
	if (MontageAttack == nullptr && MontageFrontComboEnder == nullptr) return;
	if (!bCanSwordAttack) return;
	if (CurrentEnemy == nullptr) return;
	Super::SwordAttack();

	const bool bIsEnemyFrontFacing = DetermineTargetFacingByLineTrace(GetActorLocation(), CurrentEnemy->GetActorLocation());
	TQueue<FAttackData>& AttackQ = bIsEnemyFrontFacing ? FrontAttackQueues : BackAttackQueues;
	UAnimMontage* MontageComboEnder = bIsEnemyFrontFacing ? MontageFrontComboEnder : MontageBackComboEnder;
	TArray<FAttackData> ComboEndersArray = bIsEnemyFrontFacing ? SwordAttackComboEnders : BackComboEnders;
	EnemyInteract(AttackQ, MontageAttack, ComboEndersArray, MontageComboEnder, SWORD_DAMAGE_PLAYER, bIsEnemyFrontFacing);
}

void ASTPlayerCharacter::ExecuteBlock()
{
	if (MovementState != EMovementStates::EPMS_Idle) return;
	if (MontageBlock == nullptr) return;

	PlayerAnimInstance->Montage_Play(MontageBlock);
	bCanSwordAttack = true;
	if (CurrentEnemy)
	{
		if (CurrentEnemy->IsAttacking())
		{
			if (bCanCounterAttack) bDidCounterAttack = true;
			CurrentMWPSocketName = BLOCK_SOCKET;
			OnWarpTargetUpdated();
			PlayerAnimInstance->Montage_JumpToSection(CurrentBlockSectionName, MontageBlock);
		
			CurrentEnemy->PlayNextStagger();
		}
		else {
			PlayerAnimInstance->Montage_JumpToSection(BLOCK_NO_MW, MontageBlock);
		}
	}
	else {
		PlayerAnimInstance->Montage_JumpToSection(BLOCK_NO_MW, MontageBlock);
	}

	Super::Block();
}

void ASTPlayerCharacter::ExecuteKick()
{
	if (MontageKick == nullptr && MontageKickComboEnder == nullptr) return;
	EnemyInteract(KickQueues, MontageKick, KickComboEnders, MontageKickComboEnder, KICK_DAMAGE);
}

void ASTPlayerCharacter::ExecuteCounter()
{
	if (MontageCounter == nullptr && MontageCounterComboEnder == nullptr) return;
	EnemyInteract(CounterQueues, MontageCounter, CounterComboEnders, MontageCounterComboEnder, STAGGER_DAMAGE);
}
