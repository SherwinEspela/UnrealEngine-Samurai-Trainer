// // Copyright 2024 Sherwin Espela. All rights reserved.


#include "Character/STEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

void ASTEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	EnemyAnimInstance = GetMesh()->GetAnimInstance();
}

void ASTEnemyCharacter::PlayHitReaction(FName SectionName)
{
	if (EnemyAnimInstance && MontageHitReaction)
	{
		EnemyAnimInstance->Montage_Play(MontageHitReaction);
		EnemyAnimInstance->Montage_JumpToSection(SectionName, MontageHitReaction);
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

FTransform ASTEnemyCharacter::GetAttackTransform() const
{
	return this->GetMesh()->GetSocketTransform(FName("AttackWarpSocket"), ERelativeTransformSpace::RTS_World);
}
