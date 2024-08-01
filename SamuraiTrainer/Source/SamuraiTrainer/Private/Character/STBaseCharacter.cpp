// Copyright 2024 Sherwin Espela. All rights reserved.

#include "Character/STBaseCharacter.h"
#include "Items/Katana.h"
#include "Kismet/GameplayStatics.h"
#include "SamuraiTrainer/SamuraiTrainerGameMode.h"

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

void ASTBaseCharacter::SetSlowMotion(bool IsSlow)
{
	if (IsSlow)
	{
		ASamuraiTrainerGameMode* CurrentMode = Cast<ASamuraiTrainerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (CurrentMode)
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), CurrentMode->GetSlowMotionTime());
		}
	}
	else {
		float TimeDilation = UGameplayStatics::GetGlobalTimeDilation(GetWorld());
		if (TimeDilation < 1.f)
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
		}
	}
}

void ASTBaseCharacter::HandleAttackAnimCompleted()
{
	ResetCounterAttackStates();
}

void ASTBaseCharacter::HandleStaggerAnimCompleted()
{
	ResetCounterAttackStates();
}

void ASTBaseCharacter::HandleHitReactsionAnimCompleted()
{
	ResetCounterAttackStates();
}

void ASTBaseCharacter::HandleBlockAnimCompleted()
{
	ResetCounterAttackStates();
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

void ASTBaseCharacter::ResetCounterAttackStates()
{
	bDidCounterAttack = false;
	bCanCounterAttack = false;
	MovementState = EMovementStates::EPMS_Idle;
}

