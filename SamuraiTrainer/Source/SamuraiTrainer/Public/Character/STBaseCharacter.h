// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CustomEnums.h"
#include "EnumDeathPoseType.h"
#include "EnumHitDirectionType.h"
#include "PlayerQTEResponseEnum.h"
#include "STBaseCharacter.generated.h"

class AKatana;
class ASamuraiTrainerGameMode;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttackStartedWithTwoParamsSignature, FName, BlockSectionName, FName, HRSectionName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttackStartedWith3ParamsSignature, FName, BlockSectionName, FName, HRSectionName, EPlayerQTEResponseType, PlayerQTEResposeType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackBeganSignature, EPlayerQTEResponseType, PlayerQTEResposeType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackCompletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBlockCompletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaggerStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaggerCompletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpponentWillBeDeadSignature);

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

	UFUNCTION(BlueprintCallable)
	FVector GetAttackPositionByLineTrace(FVector OtherActorLocation);

	void SetSlowMotion(bool IsSlow = true);
	virtual void SetDeathPoseType(EDeathPoseTypes Value);

	UFUNCTION(BlueprintCallable)
	void SetAttackLocationOffset(float Value);

	bool WillBeDead(float Damage) const;

public:
	// Movementments
	virtual void SwordAttack();
	virtual void Block();
	virtual void Block(FName SectionName);
	virtual void HitReact();

public:
	FORCEINLINE bool IsHealthCritical() const { return bIsHealthCritical; }
	FORCEINLINE EMovementStates GetMovementState() const { return MovementState; }
	FORCEINLINE void SetMovementState(EMovementStates Value) { MovementState = Value; }
	FORCEINLINE void SetCanCounterAttack(bool Value) { bCanCounterAttack = Value; }
	FORCEINLINE bool IsAttacking() const { return MovementState == EMovementStates::EPMS_Attacking; }
	FORCEINLINE bool DidCounterAttack() const { return bDidCounterAttack; }
	FORCEINLINE bool IsDead() const { return bIsDead; }
	FORCEINLINE void SetHitDirectionType(EHitDirectionType Value) { CurrentHitDirection = Value; }
	
public:
	FOnAttackStartedWithTwoParamsSignature OnAttackStartedWithTwoParams;
	FOnAttackStartedWith3ParamsSignature OnAttackStartedWith3Params;
	FOnAttackBeganSignature OnAttackBegan;
	FOnAttackStartedSignature OnAttackStarted;
	FOnAttackCompletedSignature OnAttackCompleted;
	FOnBlockCompletedSignature OnBlockCompleted;
	FOnStaggerStartedSignature OnStaggerStarted;
	FOnStaggerCompletedSignature OnStaggerCompleted;
	FOnOpponentWillBeDeadSignature OnOpponentWillBeDead;
	//FOnAttackHitDirectionDeterminedSignature OnAttackHitDirectionDetermined;

protected:
	virtual void BeginPlay() override;

	void AttachSwordToSocket(FName SocketName);
	bool DetermineTargetFacingByLineTrace(FVector LineTraceStart, FVector LineTraceEnd);
	EHitDirectionType DetermineHitDirectionByLineTrace(FVector LineTraceStart, FVector LineTraceEnd);
	FName GetHitReactSectionNameByHitDirection();

protected:
	// SFX
	virtual void PlaySoundSlashNoHit();
	virtual void PlaySoundSlashHit();
	virtual void PlaySoundVoiceAttack();
	virtual void PlaySoundSwordClash();

protected:
	// Animation Event Handlers
	UFUNCTION(BlueprintCallable)
	virtual void HandleAttackAnimCompleted();
	
	UFUNCTION(BlueprintCallable)
	virtual void HandleStaggerAnimCompleted();
	
	UFUNCTION(BlueprintCallable)
	virtual void HandleHitReactsionAnimCompleted();

	UFUNCTION()
	virtual void HandleBlockAnimCompleted();

	UFUNCTION()
	virtual void HandleOpponentAttackStarted(FName BlockSectionName, FName HRSectionName, EPlayerQTEResponseType ResponseType);

	UFUNCTION(BlueprintCallable)
	virtual void OnCounterAttackFrameBegan();

	UFUNCTION(BlueprintCallable)
	virtual void OnCounterAttackFrameEnded();

	void ResetCounterAttackStates();

	UFUNCTION(BlueprintCallable)
	virtual void HandleBeginSlashSound();

	UFUNCTION(BlueprintCallable)
	virtual void HandleDyingAnimationCompleted();

	UFUNCTION(BlueprintCallable)
	virtual void HandleBlockImpactEvent();

	UFUNCTION(BlueprintCallable)
	virtual void HandleBloodSpillFXNotifyBegin();

	UFUNCTION(BlueprintCallable)
	virtual void HandleBloodSpillFXNotifyEnd();

protected:
	// Character States
	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditDefaultsOnly)
	float HealthMax = 100;

	UPROPERTY(BlueprintReadOnly, Category = "Movement States")
	EMovementStates MovementState = EMovementStates::EPMS_Idle;

	bool bIsHealthCritical = false;
	bool bIsDead = false;
	bool bIsDying = false;

	UPROPERTY(EditDefaultsOnly)
	float CriticalHealthAmount = 20.f;

	UPROPERTY(EditAnywhere, Category = "Damage Amount")
	float DamageSwordAttack = 10.f;

	UPROPERTY(BlueprintReadWrite, Category = "Attack Location Offset")
	float AttackLocationOffset = 120.f;

	UPROPERTY(EditAnywhere, Category = "Attack Location Offset")
	float AttackLocationOffsetDefault = 120.f;

protected:
	// Weapon
	UPROPERTY(EditAnywhere, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AKatana> KatanaClass;

	UPROPERTY(BlueprintReadOnly, Category = "Katana")
	AKatana* Katana;

protected:
	// Animations
	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageHitReaction;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageAttackStagger;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageBlock;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageCounter;

protected:
	// SFX
	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* SoundSlashNoHit;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* SoundSlashHit;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* SoundVoiceAttack;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* SoundSwordClash;

protected:
	UPROPERTY(EditAnywhere, Category = "Debugging")
	bool bCanApplyDamage;

	FName CurrentMWPSocketName;
	APawn* CurrentTargetPawn;
	FName CurrentBlockSectionName;
	FName CurrentHRSectionName;
	bool bCanCounterAttack = false;
	bool bDidCounterAttack = false;
	bool bCanPerformNextAttack = false;
	bool bCanSwordAttack = true;
	ASamuraiTrainerGameMode* CurrentMode;
	EHitDirectionType CurrentHitDirection;
};
