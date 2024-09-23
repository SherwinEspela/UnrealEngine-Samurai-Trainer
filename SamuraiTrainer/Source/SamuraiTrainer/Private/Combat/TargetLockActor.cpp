// Copyright 2024 Sherwin Espela. All rights reserved.


#include "Combat/TargetLockActor.h"
#include "Combat/TargetLockComponent.h"
#include "Character/STPlayerCharacter.h"
#include "Character/STEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

ATargetLockActor::ATargetLockActor()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemySensorTransform = CreateDefaultSubobject<USceneComponent>(TEXT("Enemy Sensor Transform"));
	EnemySensorTransform->SetupAttachment(GetRootComponent());
}

void ATargetLockActor::BeginPlay()
{
	Super::BeginPlay();

	Player = CastChecked<ASTPlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	CollisionShape = FCollisionShape::MakeSphere(CollisionShapeRadius);
	CollisionQueryParams.AddIgnoredActor(Player);
}

void ATargetLockActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DetectEnemyByLineTrace();
}

void ATargetLockActor::SetLineEndVectors(FVector Forward, FVector Right)
{
	LineEndForwardVector = Forward;
	LineEndRightVector = Right;
}

void ATargetLockActor::DetectEnemyByLineTrace()
{
	FVector LineTraceStart = EnemySensorTransform->GetComponentLocation();
	FVector LineTraceEnd = LineTraceStart + (LineEndForwardVector * TraceDistance) + (LineEndRightVector * TraceDistance);

	FHitResult Hit;
	FQuat Rot;

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
