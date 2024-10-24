// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFMainMenuPlayerActor.generated.h"

class USkeletalMeshComponent;

UCLASS()
class SAMURAITRAINER_API ASFMainMenuPlayerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASFMainMenuPlayerActor();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display Mesh")
	USkeletalMeshComponent* Mesh;

};
