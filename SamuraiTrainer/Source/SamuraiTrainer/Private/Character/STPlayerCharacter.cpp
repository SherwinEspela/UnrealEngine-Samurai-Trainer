// // Copyright 2024 Sherwin Espela. All rights reserved.


#include "Character/STPlayerCharacter.h"
#include "Items/Katana.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimMontage.h"

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
}

void ASTPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Katana = GetWorld()->SpawnActor<AKatana>(KatanaClass);
	AttachSwordToSocket(FName("WEAPON_R"));

	bIsSwordArmed = false;
	MovementState = EMovementStates::EPMS_Default;
	WeaponState = EWeaponStates::EWS_Stored;
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

void ASTPlayerCharacter::Attack()
{
	if (bIsInteractingWithWeapon) return;
	if (MovementState == EMovementStates::EPMS_Attacking) return;
	if (WeaponState == EWeaponStates::EWS_Stored) return;
	if (MontageAttack == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(MontageAttack);
	AnimInstance->Montage_JumpToSection(FName("AttackUpslash"), MontageAttack);
	MovementState = EMovementStates::EPMS_Attacking;
}

void ASTPlayerCharacter::AttachSwordToSocket(FName SocketName)
{
	if (Katana)
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		Katana->AttachToComponent(GetMesh(), TransformRules, SocketName);
	}
}
