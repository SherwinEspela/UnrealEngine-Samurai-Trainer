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

bool ASTBaseCharacter::DetermineTargetFacingByLineTrace(FVector LineTraceStart, FVector LineTraceEnd)
{
	FHitResult Hit;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	const bool bHitSuccess = GetWorld()->LineTraceSingleByChannel(
		Hit, LineTraceStart, LineTraceEnd, ECollisionChannel::ECC_Pawn, CollisionQueryParams
	);

	bool isFacingFront = false;
	if (bHitSuccess)
	{
		const FVector EnemyForward = CurrentTargetPawn->GetActorForwardVector();
		const FVector ToHit = (Hit.ImpactPoint - CurrentTargetPawn->GetActorLocation()).GetSafeNormal();
		const double CosTheta = FVector::DotProduct(EnemyForward, ToHit);
		double Theta = FMath::Acos(CosTheta);
		Theta = FMath::RadiansToDegrees(Theta);
		isFacingFront = Theta <= 90.0f;
	}

	return isFacingFront;
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

FTransform ASTBaseCharacter::GetAttackTransform(FName SocketName) const
{
	return this->GetMesh()->GetSocketTransform(SocketName, ERelativeTransformSpace::RTS_World);
}

FName ASTBaseCharacter::GetAttackSocketName() const
{
	return CurrentMWPSocketName;
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

void ASTBaseCharacter::HandleOpponentAttackStarted(FName BlockSectionName, FName HRSectionName)
{
}

void ASTBaseCharacter::OnCounterAttackFrameBegan()
{

}

void ASTBaseCharacter::OnCounterAttackFrameEnded()
{
}

