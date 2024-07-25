// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "STStructsHolder.generated.h"

USTRUCT(BlueprintType)
struct FEnemyAttackData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Attack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Block;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USTStructsHolder : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SAMURAITRAINER_API ISTStructsHolder
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
