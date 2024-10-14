// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnumDeathPoseType.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Block;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName AttackStagger;

	EAttackType AttackType;
	EDeathPoseTypes OpponentDeathPoseType;
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	EAT_Sword		UMETA(DisplayName = "Sword"),
	EAT_Kick		UMETA(DisplayName = "Kick")
};

FORCEINLINE void PrintAttackType(EAttackType Value) {
	UE_LOG(LogTemp, Log, TEXT("Attack Type is : %s"), *UEnum::GetValueAsName(Value).ToString());
}

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ParryFatal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ParryHitReaction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ParryFatalReaction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EDeathPoseTypes DeathPoseType;
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
