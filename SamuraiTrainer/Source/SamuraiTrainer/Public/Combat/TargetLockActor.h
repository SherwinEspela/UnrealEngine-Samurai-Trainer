// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetLockActor.generated.h"

class ASTPlayerCharacter;

UCLASS()
class SAMURAITRAINER_API ATargetLockActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATargetLockActor();
	virtual void Tick(float DeltaTime) override;

	void SetLineEndVectors(FVector Forward, FVector Right);
	void SetEnabled(bool Value = true);

protected:
	virtual void BeginPlay() override;

	void DetectEnemyByLineTrace();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USceneComponent* EnemySensorTransform;

	UPROPERTY(EditAnywhere, Category = "Trace Distance")
	float TraceDistance = 500.f;

	UPROPERTY(EditAnywhere, Category = "Collision Shape Radius")
	float CollisionShapeRadius = 50.f;

	FCollisionShape CollisionShape;
	FCollisionQueryParams CollisionQueryParams;
	ASTPlayerCharacter* Player;
	FVector LineEndForwardVector;
	FVector LineEndRightVector;

private:
	bool bIsEnabled = true;

};
