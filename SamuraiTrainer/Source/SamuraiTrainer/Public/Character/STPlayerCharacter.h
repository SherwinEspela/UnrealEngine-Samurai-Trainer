// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/STBaseCharacter.h"
#include "ConstantValues.h"
#include "PlayerQTEResponseEnum.h"
#include "Structs/STStructsHolder.h"
#include "STPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class USkeletalMeshComponent;
class ASTEnemyCharacter;
class UCapsuleComponent;
class UPlayerAnimInstance;
class USceneComponent;
class ATargetLockActor;
class UNiagaraComponent;

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
	virtual void Tick(float DeltaTime) override;

	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;

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

	UFUNCTION(BlueprintCallable)
	void SetButtonsDisabled(bool Value);

	UFUNCTION()
	void HandleBasicAttackCompleted();

	virtual void OnCounterAttackFrameBegan() override;

	UFUNCTION(BlueprintCallable)
	ASTEnemyCharacter* GetTargetLockedEnemy() const;

	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	void SetCurrentEnemy(ASTEnemyCharacter* Value);
	void SetCurrentAttackingEnemyWithResponseType(ASTEnemyCharacter* Value, EPlayerQTEResponseType ResponseType);
	void RemoveCurrentAttackingEnemy();
	void SetCurrentEnemyByLineTrace(ASTEnemyCharacter* Value);
	void ToggleDebuggerDisplay();

public:
	FORCEINLINE void SetIsInteractingWithWeapon(bool Value) { bIsInteractingWithWeapon = Value; }
	FORCEINLINE EWeaponStates GetWeaponState() const { return WeaponState; }
	FORCEINLINE void SetCanPerformNextAttack(bool Value) { bCanPerformNextAttack = Value; }
	FORCEINLINE TObjectPtr<ATargetLockActor> GetTargetLockActor() const { return TargetLockActor; }

public:
	// Movmentments
	void SwordInteract();
	virtual void SwordAttack() override;
	virtual void SwordAttackCombo2();
	void Kick();
	void Counter();
	virtual void Block() override;
	virtual void HitReact() override;
	virtual void PlayAttackStagger(FName SectionName) override;

protected:
	virtual void BeginPlay() override;

	void InitPlayerAnimInstance();
	void InitQueues();
	void EnemyInteract(
		TQueue<FAttackData> &MoveQueue, 
		UAnimMontage* MontageToPlay, 
		TArray<FAttackData> ComboEnders, 
		UAnimMontage* MontageEnder, 
		float Damage,
		bool IsEnemyFacingFront = true
	);

protected:
	// Animation Event Handlers
	virtual void HandleAttackAnimCompleted() override;

	UFUNCTION()
	virtual void HandleEnemyCanBlockEvent();

	UFUNCTION()
	virtual void HandleEnemyCanHitReactEvent();

	virtual void HandleBeginSlashSound() override;
	virtual void HandleBloodSpillFXNotifyBegin() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void HandleWeaponToLeftHand();

	UFUNCTION(BlueprintCallable)
	virtual void HandleWeaponToRightHand();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Camera Setup")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category = "Camera Setup")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon States")
	EWeaponStates WeaponState = EWeaponStates::EWS_Stored;

	UPROPERTY(EditAnywhere, Category = "Current Enemy")
	ASTEnemyCharacter* CurrentEnemy;

	UPROPERTY(EditAnywhere, Category = "Current Enemy")
	float DistanceFromEnemyAllowed = 400.f;

	ASTEnemyCharacter* CurrentAttackingEnemy;

	TQueue<FAttackData> FrontAttackQueues;
	TQueue<FAttackData> AttackCombo2Queues;
	TQueue<FAttackData> BackAttackQueues;
	TQueue<FAttackData> KickQueues;
	TQueue<FAttackData> CounterQueues;

	TArray<FAttackData> SwordAttackComboEnders;
	TArray<FAttackData> SwordAttackComboEnders2;
	TArray<FAttackData> BackComboEnders;
	TArray<FAttackData> KickComboEnders;
	TArray<FAttackData> CounterComboEnders;
	TArray<FAttackData> AttackCounterList;

	FAttackData CurrentAttackData;
	EAttackType CurrentAttackType;

protected:
	// Animations
	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageSwordInteract;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageAttackCombo2;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageFrontComboEnder;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageComboEnder2;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageBackComboEnder;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageKickComboEnder;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageCounterComboEnder;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageKick;

	FName NextAttackSectionName = ATTACK_DOWNSLASH;
	FName NextHitReactionSectionName = HIT_REACTION_DOWNSLASH;

	bool bIsSwordArmed = false;
	bool bIsInteractingWithWeapon = false;
	bool bIsLastBasicAttack = false;
	bool bIsQTEMode = false;
	
	UPROPERTY(EditAnywhere, Category = "Debugging")
	bool bDebugEnemyCannotCounterAttack = false;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Block or Evade Chance")
	int EnemyBlockOrEvadeChance = 50;

	TArray<FName> BlockSectionNames;

protected:
	UFUNCTION()
	void OnEnemyDetectorBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult
	);

	virtual void HandleOpponentAttackStarted(FName BlockSectionName, FName HRSectionName, EPlayerQTEResponseType ResponseType) override;

protected:
	// QTE
	EPlayerQTEResponseType CurrentPlayerQTEResponse;
	EPlayerQTEResponseType ExpectedPlayerQTEResponse;
	virtual void QTEResult();

private:
	// Movement Execution
	void ExecuteSwordAttack();
	void ExecuteSwordAttackCombo2();
	void ExecuteBlock();
	void ExecuteKick();
	void ExecuteCounter();

protected:
	// Combat
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<ATargetLockActor> TargetLockActorClass;

	TObjectPtr<ATargetLockActor> TargetLockActor;

	UPROPERTY(EditAnywhere, Category = "Combat")
	bool bIsDebuggerDisplayed = true;

protected:
	// FX
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UNiagaraComponent* FXTargetBeam;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	float TargetBeamWidth = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	float TargetBeamActiveDistance = 500.f;

private:
	UPlayerAnimInstance* PlayerAnimInstance;
	bool bEnemyCanBlockOrEvade;
	bool bButtonsDisabled = false;
};
