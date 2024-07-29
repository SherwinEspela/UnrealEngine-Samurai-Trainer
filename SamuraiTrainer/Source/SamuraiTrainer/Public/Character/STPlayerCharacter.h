// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/STBaseCharacter.h"
#include "ConstantValues.h"
#include "Structs/STStructsHolder.h"
#include "STPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class USkeletalMeshComponent;
class ASTEnemyCharacter;
class UCapsuleComponent;
class UPlayerAnimInstance;

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API ASTPlayerCharacter : public ASTBaseCharacter
{
	GENERATED_BODY()
public:
	ASTPlayerCharacter();

public:
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;

	void SwordInteract();
	void Attack();
	void Block();
	void Kick();
	void Counter();

	UFUNCTION(BlueprintCallable)
	void OnComboFrameBegan(bool IsLastBasicAttack);

	UFUNCTION(BlueprintCallable)
	void OnComboFrameEnded();

	UFUNCTION(BlueprintCallable)
	void OnComboEnderStarted();

	UFUNCTION(BlueprintCallable)
	void OnComboEnderCompleted();

	UFUNCTION(BlueprintCallable)
	void SetNextAttackSectionName(FName Value);

	UFUNCTION(BlueprintCallable)
	void SetNextHitReactionSectionName(FName Value);

	UFUNCTION()
	void HandleBasicAttackCompleted();

	UFUNCTION(BlueprintCallable)
	ASTEnemyCharacter* GetTargetLockedEnemy() const;

	UFUNCTION(BlueprintCallable)
	FName GetAttackSocketName() const;

public:
	FORCEINLINE void SetIsInteractingWithWeapon(bool Value) { bIsInteractingWithWeapon = Value; }
	FORCEINLINE EWeaponStates GetWeaponState() const { return WeaponState; }
	FORCEINLINE void SetCanPerformNextAttack(bool Value) { bCanPerformNextAttack = Value; }

protected:
	virtual void BeginPlay() override;

	void InitQueues();
	void EnemyInteract(
		TQueue<FAttackData> &MoveQueue, 
		UAnimMontage* MontageToPlay, 
		TArray<FAttackData> ComboEnders, 
		UAnimMontage* MontageEnder, 
		float Damage,
		bool IsEnemyFacingFront = true
	);
	bool DetermineEnemyFacingByLineTrace(FVector LineTraceStart, FVector LineTraceEnd);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Camera Setup")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category = "Camera Setup")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon States")
	EWeaponStates WeaponState = EWeaponStates::EWS_Stored;

	UPROPERTY(EditAnywhere, Category = "Current Enemy")
	ASTEnemyCharacter* CurrentEnemy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Detector")
	UCapsuleComponent* CapsuleEnemyDetector;

	TQueue<FAttackData> FrontAttackQueues;
	TQueue<FAttackData> BackAttackQueues;
	TQueue<FAttackData> KickQueues;
	TQueue<FAttackData> CounterQueues;

	TArray<FAttackData> SwordAttackComboEnders;
	TArray<FAttackData> BackComboEnders;
	TArray<FAttackData> KickComboEnders;
	TArray<FAttackData> CounterComboEnders;
	TArray<FAttackData> AttackCounterList;

protected:
	// Animations
	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageSwordInteract;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageFrontComboEnder;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageBackComboEnder;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageKickComboEnder;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageCounterComboEnder;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageBlock;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageKick;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageCounter;

	FName NextAttackSectionName = ATTACK_DOWNSLASH;
	FName NextHitReactionSectionName = HIT_REACTION_DOWNSLASH;

	bool bIsSwordArmed = false;
	bool bIsInteractingWithWeapon = false;
	bool bCanPerformNextAttack = false;
	bool bIsLastBasicAttack = false;

	UPROPERTY(EditDefaultsOnly, Category = "Sword Damage Amount")
	float DamageAmount = 30.f;

protected:
	UFUNCTION()
	void OnEnemyDetectorBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnEnemyAttackStartedWithTwoParams(FName BlockSectionName, FName StaggerSectionName);

private:
	UPlayerAnimInstance* PlayerAnimInstance;
	FName CurrentAttackSocketName;
	FName CurrentBlockSocketName;
	FName NextEnemyStaggerSocketName;

};
