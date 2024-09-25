// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetIndicatorActor.generated.h"

class UNiagaraComponent;

UCLASS()
class SAMURAITRAINER_API ATargetIndicatorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATargetIndicatorActor();
	virtual void Tick(float DeltaTime) override;

	void DisplayIndicator(bool ShouldDisplay);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraComponent* FXIndicator;

};
