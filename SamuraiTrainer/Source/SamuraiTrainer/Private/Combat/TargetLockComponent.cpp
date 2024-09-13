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

	CollisionShape = FCollisionShape::MakeSphere(CollisionShapeRadius);
	CollisionQueryParams.AddIgnoredActor(GetOwner());
}

void UTargetLockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DetectEnemyByLineTrace();
}

void UTargetLockComponent::DetectEnemyByLineTrace()
{
	if (!bIsEnabled) return;
	if (LineTraceOriginTransform == nullptr) return;

	FVector LineTraceStart = LineTraceOriginTransform->GetComponentLocation();
	FVector LineTraceEnd = LineTraceStart + (LineTraceOriginTransform->GetForwardVector() * TraceDistance);

	DrawDebugLine(GetWorld(), LineTraceStart, LineTraceEnd, FColor::Green);

	FHitResult Hit;
	FQuat Rot;
	/*const bool bHitSuccess = GetWorld()->LineTraceSingleByChannel(
		Hit, LineTraceStart, LineTraceEnd, ECollisionChannel::ECC_Pawn, CollisionQueryParams
	);*/

	const bool bHitSuccess = GetWorld()->SweepSingleByChannel(
		Hit, LineTraceStart, LineTraceEnd, Rot, ECollisionChannel::ECC_Pawn, 
		CollisionShape, CollisionQueryParams);

	if (bHitSuccess)
	{
		ASTEnemyCharacter* Enemy = Cast<ASTEnemyCharacter>(Hit.GetActor());
		if (Player && Enemy && !Enemy->IsDead())
		{
			Player->SetCurrentEnemyByLineTrace(Enemy);
		}
	}
}
