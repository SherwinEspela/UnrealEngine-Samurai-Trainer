// Copyright 2024 Sherwin Espela. All rights reserved.

#include "Character/STBaseCharacter.h"
#include "Items/Katana.h"
#include "Kismet/GameplayStatics.h"
#include "SamuraiTrainer/SamuraiTrainerGameMode.h"
#include "Components/ActorComponent.h"

ASTBaseCharacter::ASTBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void ASTBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = HealthMax;
	MovementState = EMovementStates::EPMS_Idle;

	if (KatanaClass)
	{
		Katana = GetWorld()->SpawnActor<AKatana>(KatanaClass);
	}

	CurrentMode = Cast<ASamuraiTrainerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	AttackLocationOffset = AttackLocationOffsetDefault;
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
	if (bHitSuccess && CurrentTargetPawn)
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

EHitDirectionType ASTBaseCharacter::DetermineHitDirectionByLineTrace(FVector LineTraceStart, FVector LineTraceEnd)
{
	if (CurrentTargetPawn == nullptr) return EHitDirectionType::EHDT_Front;

	FHitResult Hit;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	const bool bHitSuccess = GetWorld()->LineTraceSingleByChannel(
		Hit, LineTraceStart, LineTraceEnd, ECollisionChannel::ECC_Pawn, CollisionQueryParams
	);

	EHitDirectionType HitDirection;
	if (bHitSuccess)
	{
		const FVector EnemyForward = CurrentTargetPawn->GetActorForwardVector();
		const FVector ToHit = (Hit.ImpactPoint - CurrentTargetPawn->GetActorLocation()).GetSafeNormal();
		const double CosTheta = FVector::DotProduct(EnemyForward, ToHit);
		double Theta = FMath::Acos(CosTheta);
		Theta = FMath::RadiansToDegrees(Theta);

		const FVector CrossProduct = FVector::CrossProduct(EnemyForward, ToHit);
		if (CrossProduct.Z < 0)
		{
			Theta *= -1;
		}

		if (Theta <= 45.f && Theta >= -45.f)
		{
			HitDirection = EHitDirectionType::EHDT_Front;
		}
		else if (Theta > 45.f && Theta <= 135.f)
		{
			HitDirection = EHitDirectionType::EHDT_Right;
		}
		else if (Theta < -45.f && Theta >= -135.f)
		{
			HitDirection = EHitDirectionType::EHDT_Left;
		}
		else if (Theta > 135.f || Theta < -135.f)
		{
			HitDirection = EHitDirectionType::EHDT_Back;
		}
	}

	return HitDirection;
}

FName ASTBaseCharacter::GetHitReactSectionNameByHitDirection()
{
	FName SectionName = "HRFront";

	switch (CurrentHitDirection)
	{
	case EHitDirectionType::EHDT_Front:
		SectionName = "HRFront";
		break;
	case EHitDirectionType::EHDT_Back:
		SectionName = "HRBack";
		break;
	case EHitDirectionType::EHDT_Left:
		SectionName = "HRLeft";
		break;
	case EHitDirectionType::EHDT_Right:
		SectionName = "HRRight";
		break;
	default:
		break;
	}

	return SectionName;
}

void ASTBaseCharacter::PlaySoundSlashNoHit()
{
	if (SoundSlashNoHit == nullptr) return;
	UGameplayStatics::PlaySound2D(this, SoundSlashNoHit);
}

void ASTBaseCharacter::PlaySoundSlashHit()
{
	if (SoundSlashHit == nullptr) return;
	UGameplayStatics::PlaySound2D(this, SoundSlashHit);
}

void ASTBaseCharacter::PlaySoundVoiceAttack()
{
	if (SoundVoiceAttack == nullptr) return;
	UGameplayStatics::PlaySound2D(this, SoundVoiceAttack);
}

void ASTBaseCharacter::PlaySoundSwordClash()
{
	if (SoundSwordClash == nullptr) return;
	UGameplayStatics::PlaySound2D(this, SoundSwordClash);

	if (Katana)
	{
		Katana->PlaySwordClashFx();
	}
}

void ASTBaseCharacter::SwordAttack()
{
	AttackLocationOffset = AttackLocationOffsetDefault;
	MovementState = EMovementStates::EPMS_SwordAttacking;
}

void ASTBaseCharacter::Block()
{
	MovementState = EMovementStates::EPMS_Blocking;
}

void ASTBaseCharacter::Block(FName SectionName)
{
	MovementState = EMovementStates::EPMS_Blocking;
}

void ASTBaseCharacter::HitReact()
{
	MovementState = EMovementStates::EPMS_HitReacting;
}

void ASTBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ASTBaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (bCanApplyDamage)
	{
		DamageApplied = FMath::Min(Health, DamageApplied);
		Health -= DamageApplied;
		bIsHealthCritical = Health > 0 && Health <= CriticalHealthAmount;
		bIsDying = bIsHealthCritical;
		bIsDead = Health <= 0.f;
	}
	
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

FVector ASTBaseCharacter::GetAttackPositionByLineTrace(FVector OtherActorLocation)
{
	FVector LineTraceStart = GetActorLocation();
	FVector AttackLocation;

	FHitResult Hit;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	const bool bHitSuccess = GetWorld()->LineTraceSingleByChannel(
		Hit, LineTraceStart, OtherActorLocation, ECollisionChannel::ECC_Pawn, CollisionQueryParams
	);

	if (bHitSuccess)
	{		
		AttackLocation = OtherActorLocation + (Hit.ImpactNormal * AttackLocationOffset);
	}

	return AttackLocation;
}

void ASTBaseCharacter::SetSlowMotion(bool IsSlow)
{
	if (IsSlow)
	{
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

void ASTBaseCharacter::SetDeathPoseType(EDeathPoseTypes Value)
{
}

void ASTBaseCharacter::SetAttackLocationOffset(float Value)
{
	AttackLocationOffset = Value;
	OnWarpTargetUpdated();
}

bool ASTBaseCharacter::WillBeDead(float Damage) const
{
	bool WillDie = Health <= Damage;
	if (WillDie) OnOpponentWillBeDead.Broadcast();
	return WillDie;
}

// Called in Anim Blueprint
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

void ASTBaseCharacter::HandleOpponentAttackStarted(FName BlockSectionName, FName HRSectionName, EPlayerQTEResponseType ResponseType)
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
	MovementState = EMovementStates::EPMS_Idle;
	bDidCounterAttack = false;
	bCanCounterAttack = false;
	bCanPerformNextAttack = true;
	bCanSwordAttack = true;
	AttackLocationOffset = AttackLocationOffsetDefault;
}

void ASTBaseCharacter::HandleBeginSlashSound()
{
}

void ASTBaseCharacter::HandleDyingAnimationCompleted()
{
}

void ASTBaseCharacter::HandleBlockImpactEvent()
{
	PlaySoundSwordClash();
}

void ASTBaseCharacter::HandleBloodSpillFXNotifyBegin()
{
	
}

void ASTBaseCharacter::HandleBloodSpillFXNotifyEnd()
{
	if (Katana) Katana->ShouldPlayBloodSpillFx(false);
}
