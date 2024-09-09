// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetLockComponent.generated.h"

class USceneComponent;
class ASTPlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAMURAITRAINER_API UTargetLockComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTargetLockComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE void SetLineTraceOrigin(USceneComponent* Value) { LineTraceOriginTransform = Value; }
	FORCEINLINE void SetEnabled(bool Value = true) { bIsEnabled = Value; }

protected:
	virtual void BeginPlay() override;

	void DetectEnemyByLineTrace();

	UPROPERTY(EditAnywhere, Category = "Debugging")
	bool bIsEnabled = true;

private:
	ASTPlayerCharacter* Player;
	USceneComponent* LineTraceOriginTransform;

};