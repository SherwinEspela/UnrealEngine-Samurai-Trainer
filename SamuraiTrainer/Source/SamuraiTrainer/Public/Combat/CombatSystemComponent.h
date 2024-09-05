// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatSystemComponent.generated.h"

class ASTPlayerCharacter;
class ASTEnemyCharacter;

UCLASS()
class SAMURAITRAINER_API ACombatSystemComponent : public AActor
{
	GENERATED_BODY()
	
public:	
	ACombatSystemComponent();

protected:
	virtual void BeginPlay() override;

protected:
	TObjectPtr<ASTPlayerCharacter> Player;

	UPROPERTY(EditAnywhere, Category = "Enemy Character")
	TObjectPtr<ASTEnemyCharacter> Enemy;
};
