// Copyright 2024 Sherwin Espela. All rights reserved.


#include "Character/STPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Character/STEnemyCharacter.h"
#include "Animations/PlayerAnimInstance.h"
#include "Engine/DamageEvents.h"

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
	MovementState = EMovementStates::EPMS_Default;
	WeaponState = EWeaponStates::EWS_Stored;
	bCanPerformNextAttack = true;

	PlayerAnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (PlayerAnimInstance)
	{
		PlayerAnimInstance->OnAttackAnimCompleted.AddDynamic(this, &ASTPlayerCharacter::HandleBasicAttackCompleted);
	}

	if (CapsuleEnemyDetector)
	{
		CapsuleEnemyDetector->OnComponentBeginOverlap.AddDynamic(this, &ASTPlayerCharacter::OnEnemyDetectorBeginOverlap);
	}

	InitQueues();
}

void ASTPlayerCharacter::InitQueues()
{
	FrontAttackQueues.Enqueue(FAttackData(ATTACK_DOWNSLASH, HIT_REACTION_DOWNSLASH, ATTACK_SOCKET_FRONT));
	FrontAttackQueues.Enqueue(FAttackData(ATTACK_UPSLASH, HIT_REACTION_UPSLASH, ATTACK_SOCKET_FRONT));
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
		if (CurrentEnemy->IsHealthCritical())
		{
			AttackData = ComboEnders[FMath::RandRange(0, ComboEnders.Num() - 1)];
			PlayerAnimInstance->Montage_Play(MontageEnder);
			PlayerAnimInstance->Montage_JumpToSection(AttackData.Attack, MontageEnder);
		}
		else {
			PlayerAnimInstance->Montage_Play(MontageToPlay);
			PlayerAnimInstance->Montage_JumpToSection(AttackData.Attack, MontageToPlay);
		}

		CurrentMWPSocketName = AttackData.AttackSocketName;
		OnWarpTargetUpdated();
		CurrentEnemy->SetNextHitReactionSectionName(AttackData.HitReaction);
		FDamageEvent DamageEvent;
		CurrentEnemy->TakeDamage(Damage, DamageEvent, GetController(), this);
	}
	else {
		PlayerAnimInstance->Montage_Play(MontageToPlay);
		PlayerAnimInstance->Montage_JumpToSection(AttackData.Attack, MontageToPlay);
	}

	MoveQueue.Enqueue(AttackData);

	MovementState = EMovementStates::EPMS_Attacking;
	bCanPerformNextAttack = false;
}

void ASTPlayerCharacter::Attack()
{
	if (CurrentEnemy == nullptr) return;
	if (MontageAttack == nullptr && MontageFrontComboEnder == nullptr) return;
	
	const bool bIsEnemyFrontFacing = DetermineTargetFacingByLineTrace(GetActorLocation(), CurrentEnemy->GetActorLocation());
	TQueue<FAttackData>& AttackQ = bIsEnemyFrontFacing ? FrontAttackQueues : BackAttackQueues;
	UAnimMontage* MontageComboEnder = bIsEnemyFrontFacing ? MontageFrontComboEnder : MontageBackComboEnder;
	TArray<FAttackData> ComboEndersArray = bIsEnemyFrontFacing ? SwordAttackComboEnders : BackComboEnders;
	EnemyInteract(AttackQ, MontageAttack, ComboEndersArray, MontageComboEnder, 30.f, bIsEnemyFrontFacing);
}

void ASTPlayerCharacter::Block()
{
	if (MontageBlock == nullptr) return;

	PlayerAnimInstance->Montage_Play(MontageBlock);
	if (CurrentEnemy)
	{
		bool isAttacking = CurrentEnemy->GetMovementState() == EMovementStates::EPMS_Attacking;
		if (isAttacking)
		{
			CurrentMWPSocketName = BLOCK_SOCKET;
			OnWarpTargetUpdated();
			PlayerAnimInstance->Montage_JumpToSection(CurrentBlockSectionName, MontageBlock);
			CurrentEnemy->PlayNextStagger();
		}
		else {
			PrintMovementState(CurrentEnemy->GetMovementState());
			PlayerAnimInstance->Montage_JumpToSection(BLOCK_NO_MW, MontageBlock);
		}
	}
	else {
		PlayerAnimInstance->Montage_JumpToSection(BLOCK_NO_MW, MontageBlock);
	}
}

void ASTPlayerCharacter::Kick()
{
	if (MontageKick == nullptr && MontageKickComboEnder == nullptr) return;
	EnemyInteract(KickQueues, MontageKick, KickComboEnders, MontageKickComboEnder, 10.f);
}

void ASTPlayerCharacter::Counter()
{
	if (MontageCounter == nullptr && MontageCounterComboEnder == nullptr) return;
	EnemyInteract(CounterQueues, MontageCounter, CounterComboEnders, MontageCounterComboEnder, 10.f);
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

void ASTPlayerCharacter::HandleBasicAttackCompleted()
{
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
	if (PlayerAnimInstance)
	{
		PlayerAnimInstance->Montage_Play(MontageHitReaction);
		PlayerAnimInstance->Montage_JumpToSection(CurrentHRSectionName, MontageHitReaction);
	}

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
			CurrentEnemy->OnAttackStartedWithTwoParams.AddDynamic(this, &ASTPlayerCharacter::HandleOpponentAttackStarted);
			CurrentTargetPawn = CurrentEnemy;
		}
	}
}

void ASTPlayerCharacter::HandleOpponentAttackStarted(FName BlockSectionName, FName HRSectionName)
{
	CurrentBlockSectionName = BlockSectionName;
	CurrentHRSectionName = HRSectionName;
}
