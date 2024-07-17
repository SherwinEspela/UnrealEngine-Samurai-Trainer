// // Copyright 2024 Sherwin Espela. All rights reserved.


#include "Character/STEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

//ASTEnemyCharacter::ASTEnemyCharacter()
//{
//	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing"));
//}

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
	PlayerPawn = Target;
	OnWarpTargetUpdated();
}
