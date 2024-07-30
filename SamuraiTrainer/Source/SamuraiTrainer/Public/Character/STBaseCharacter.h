// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CustomEnums.h"
#include "STBaseCharacter.generated.h"

class AKatana;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackStartedSignature, FName, BlockSectionName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttackStartedWithTwoParamsSignature, FName, BlockSectionName, FName, StaggerSectionName);

UCLASS()
class SAMURAITRAINER_API ASTBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASTBaseCharacter();
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

public:
	void ReduceHealth(int Value);

	UFUNCTION(BlueprintImplementableEvent)
	void OnWarpTargetUpdated();

	virtual void PlayAttackStagger(FName SectionName);

	UFUNCTION(BlueprintCallable)
	FTransform GetAttackTransform(FName SocketName) const;

	UFUNCTION(BlueprintCallable)
	FName GetAttackSocketName() const;

public:
	FORCEINLINE bool IsHealthCritical() const { return bIsHealthCritical; }
	FORCEINLINE EMovementStates GetMovementState() const { return MovementState; }
	FORCEINLINE void SetMovementState(EMovementStates Value) { MovementState = Value; }

public:
	FOnAttackStartedSignature OnAttackStarted;
	FOnAttackStartedWithTwoParamsSignature OnAttackStartedWithTwoParams;

protected:
	virtual void BeginPlay() override;

	void AttachSwordToSocket(FName SocketName);

	bool DetermineTargetFacingByLineTrace(FVector LineTraceStart, FVector LineTraceEnd);

protected:
	// Animation Event Handlers
	UFUNCTION()
	virtual void HandleAttackAnimCompleted();
	
	UFUNCTION()
	virtual void HandleStaggerAnimCompleted();
	
	UFUNCTION()
	virtual void HandleHitReactsionAnimCompleted();

protected:
	// Character States
	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditDefaultsOnly)
	float HealthMax = 100;

	UPROPERTY(BlueprintReadOnly, Category = "Movement States")
	EMovementStates MovementState = EMovementStates::EPMS_Default;

	bool bIsHealthCritical = false;

	UPROPERTY(EditDefaultsOnly)
	float CriticalHealthAmount = 20.f;

protected:
	// Weapon
	UPROPERTY(EditAnywhere, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AKatana> KatanaClass;

	AKatana* Katana;

protected:
	// Animations
	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageAttackStagger;

protected:
	FName CurrentAttackSocketName;
	APawn* CurrentTargetPawn;

};
