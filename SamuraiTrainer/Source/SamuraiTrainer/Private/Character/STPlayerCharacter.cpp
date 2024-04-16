// // Copyright 2024 Sherwin Espela. All rights reserved.


#include "Character/STPlayerCharacter.h"
#include "Items/Katana.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimMontage.h"

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
}

void ASTPlayerCharacter::SwordInteract()
{
	if (MontageSwordInteract)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(MontageSwordInteract);

		/*if (bIsSwordArmed)
		{
			AnimInstance->Montage_JumpToSection(FName("Sheathe"), MontageSwordInteract);
		}
		else {
			AnimInstance->Montage_JumpToSection(FName("Unsheathe"), MontageSwordInteract);
		}*/

		FName SectionName = bIsSwordArmed ? FName("Unsheathe") : FName("Sheathe");
		AnimInstance->Montage_JumpToSection(SectionName, MontageSwordInteract);
		bIsSwordArmed = !bIsSwordArmed;
	}
}

void ASTPlayerCharacter::AttachSwordToSocket(FName SocketName)
{
	if (Katana)
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		Katana->AttachToComponent(GetMesh(), TransformRules, SocketName);
	}
}
