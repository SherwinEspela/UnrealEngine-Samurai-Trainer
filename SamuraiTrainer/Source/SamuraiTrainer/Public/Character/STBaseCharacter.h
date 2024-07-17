// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STBaseCharacter.generated.h"

UCLASS()
class SAMURAITRAINER_API ASTBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASTBaseCharacter();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnWarpTargetUpdated();

protected:
	virtual void BeginPlay() override;
};
