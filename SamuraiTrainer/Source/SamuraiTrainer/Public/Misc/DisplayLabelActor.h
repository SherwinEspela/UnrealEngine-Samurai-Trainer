// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DisplayLabelActor.generated.h"

class UTextRenderComponent;

UCLASS()
class SAMURAITRAINER_API ADisplayLabelActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADisplayLabelActor();

	void DisplayLabels(bool ShouldDisplay = true);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Labels")
	TArray<AActor*> Labels;
};
