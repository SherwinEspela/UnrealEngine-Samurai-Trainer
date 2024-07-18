// // Copyright 2024 Sherwin Espela. All rights reserved.


#include "Character/STPlayerCharacter.h"
#include "Items/Katana.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Character/STEnemyCharacter.h"
#include "Engine/DamageEvents.h"

#define STRING_SHEATHE FName("Sheathe")
#define STRING_UNSHEATHE FName("Unsheathe")

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

	Katana = GetWorld()->SpawnActor<AKatana>(KatanaClass);
	AttachSwordToSocket(FName("WEAPON_R"));

	bIsSwordArmed = false;
	MovementState = EMovementStates::EPMS_Default;
	WeaponState = EWeaponStates::EWS_Stored;
	bCanPerformNextAttack = true;
	PlayerAnimInstance = GetMesh()->GetAnimInstance();

	if (CapsuleEnemyDetector)
	{
		CapsuleEnemyDetector->OnComponentBeginOverlap.AddDynamic(this, &ASTPlayerCharacter::OnEnemyDetectorBeginOverlap);
	}

	InitQueues();
}

void ASTPlayerCharacter::InitQueues()
{
	AttackQueues.Enqueue(FAttackData(ATTACK_DOWNSLASH, HIT_REACTION_DOWNSLASH));
	AttackQueues.Enqueue(FAttackData(ATTACK_UPSLASH, HIT_REACTION_UPSLASH));

	KickQueues.Enqueue(FAttackData(KICK1, HIT_REACTION_KICK1));
	KickQueues.Enqueue(FAttackData(KICK2, HIT_REACTION_KICK2));
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

void ASTPlayerCharacter::Attack()
{
	if (!bCanPerformNextAttack) return;
	if (PlayerAnimInstance == nullptr) return;

	FAttackData AttackData;
	AttackQueues.Dequeue(AttackData);

	if (CurrentEnemy)
	{
		OnWarpTargetUpdated();

		if (CurrentEnemy->IsHealthCritical())
		{
			PlayerAnimInstance->Montage_Play(MontageComboEnder);
			PlayerAnimInstance->Montage_JumpToSection(ATTACK_COMBO_END1, MontageComboEnder);
		}
		else {
			PlayerAnimInstance->Montage_Play(MontageAttack);
			PlayerAnimInstance->Montage_JumpToSection(AttackData.Attack, MontageAttack);
		}

		CurrentEnemy->UpdateWarpTarget(this);
		CurrentEnemy->SetNextHitReactionSectionName(AttackData.HitReaction);
		FDamageEvent DamageEvent;
		CurrentEnemy->TakeDamage(DamageAmount, DamageEvent, GetController(), this);
	}
	else {
		PlayerAnimInstance->Montage_Play(MontageAttack);
		PlayerAnimInstance->Montage_JumpToSection(AttackData.Attack, MontageAttack);
	}

	AttackQueues.Enqueue(AttackData);

	MovementState = EMovementStates::EPMS_Attacking;
	bCanPerformNextAttack = false;
}

void ASTPlayerCharacter::Block()
{
	PlayerAnimInstance->Montage_Play(MontageBlock);
	PlayerAnimInstance->Montage_JumpToSection(FName("1"), MontageBlock);
}

void ASTPlayerCharacter::Kick()
{
	if (!bCanPerformNextAttack) return;
	if (PlayerAnimInstance == nullptr) return;

	FAttackData KickData;
	KickQueues.Dequeue(KickData);

	if (CurrentEnemy)
	{
		OnWarpTargetUpdated();

		if (CurrentEnemy->IsHealthCritical())
		{
			PlayerAnimInstance->Montage_Play(MontageComboEnder);
			PlayerAnimInstance->Montage_JumpToSection(ATTACK_COMBO_END1, MontageComboEnder);
		}
		else {
			PlayerAnimInstance->Montage_Play(MontageKick);
			PlayerAnimInstance->Montage_JumpToSection(KickData.Attack, MontageKick);
		}

		CurrentEnemy->UpdateWarpTarget(this);
		CurrentEnemy->SetNextHitReactionSectionName(KickData.HitReaction);
		FDamageEvent DamageEvent;
		CurrentEnemy->TakeDamage(10.f, DamageEvent, GetController(), this);
	}
	else {
		PlayerAnimInstance->Montage_Play(MontageKick);
		PlayerAnimInstance->Montage_JumpToSection(KickData.Attack, MontageKick);
	}

	KickQueues.Enqueue(KickData);

	MovementState = EMovementStates::EPMS_Attacking;
	bCanPerformNextAttack = false;
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

ASTEnemyCharacter* ASTPlayerCharacter::GetTargetLockedEnemy() const
{
	return CurrentEnemy;
}

void ASTPlayerCharacter::AttachSwordToSocket(FName SocketName)
{
	if (Katana)
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		Katana->AttachToComponent(GetMesh(), TransformRules, SocketName);
	}
}

void ASTPlayerCharacter::OnEnemyDetectorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CurrentEnemy && OtherActor->GetName() == CurrentEnemy->GetName()) return;
	if (OtherActor) CurrentEnemy = Cast<ASTEnemyCharacter>(OtherActor);
}
