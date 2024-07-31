// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "STStructsHolder.generated.h"

USTRUCT(BlueprintType)
struct FAttackData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Attack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName HitReaction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName AttackSocketName;
};

USTRUCT(BlueprintType)
struct FEnemyAttackData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Attack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NextPlayerBlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NextStagger;
};

USTRUCT(BlueprintType)
struct FAttackAndCounterReactionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Attack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName CounterBlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName HitReaction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName CBStagger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MWPSocketName;
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
