// // Copyright 2024 Sherwin Espela. All rights reserved.


#include "Combat/TargetLockComponent.h"
#include "Character/STEnemyCharacter.h"
#include "Character/STPlayerCharacter.h"

UTargetLockComponent::UTargetLockComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTargetLockComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = CastChecked<ASTPlayerCharacter>(GetOwner());
}

void UTargetLockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DetectEnemyByLineTrace();
}

void UTargetLockComponent::DetectEnemyByLineTrace()
{
	//if (!bIsEnabled) return;
	if (LineTraceOriginTransform == nullptr) return;

	FVector LineTraceStart = LineTraceOriginTransform->GetComponentLocation();
	FVector LineTraceEnd = LineTraceStart + (LineTraceOriginTransform->GetForwardVector() * 5000.f);

	FHitResult Hit;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	const bool bHitSuccess = GetWorld()->LineTraceSingleByChannel(
		Hit, LineTraceStart, LineTraceEnd, ECollisionChannel::ECC_Pawn, CollisionQueryParams
	);

	if (bHitSuccess)
	{
		ASTEnemyCharacter* Enemy = Cast<ASTEnemyCharacter>(Hit.GetActor());
		if (Player && Enemy && !Enemy->IsDead())
		{
			Player->SetCurrentEnemyByLineTrace(Enemy);
		}
	}
}
