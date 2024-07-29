// Copyright 2024 Sherwin Espela. All rights reserved.

#include "Character/STBaseCharacter.h"
#include "Items/Katana.h"

ASTBaseCharacter::ASTBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASTBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Health = HealthMax;

	if (KatanaClass)
	{
		Katana = GetWorld()->SpawnActor<AKatana>(KatanaClass);
		AttachSwordToSocket(FName("WEAPON_R"));
	}
}

void ASTBaseCharacter::AttachSwordToSocket(FName SocketName)
{
	if (Katana)
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		Katana->AttachToComponent(GetMesh(), TransformRules, SocketName);
	}
}

void ASTBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ASTBaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageApplied = FMath::Min(Health, DamageApplied);
	Health -= DamageApplied;
	bIsHealthCritical = Health <= CriticalHealthAmount;

	return DamageApplied;
}

void ASTBaseCharacter::ReduceHealth(int Value)
{
	Health -= Value;
}

void ASTBaseCharacter::PlayAttackStagger(FName SectionName)
{
	if (MontageAttackStagger == nullptr) return;
	MovementState = EMovementStates::EPMS_Staggered;
}

void ASTBaseCharacter::HandleAttackAnimCompleted()
{
	MovementState = EMovementStates::EPMS_Idle;
}

void ASTBaseCharacter::HandleStaggerAnimCompleted()
{
	MovementState = EMovementStates::EPMS_Idle;
}

void ASTBaseCharacter::HandleHitReactsionAnimCompleted()
{
	MovementState = EMovementStates::EPMS_Idle;
}
