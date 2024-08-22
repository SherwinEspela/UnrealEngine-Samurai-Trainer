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

public:
	FORCEINLINE void SetIsInteractingWithWeapon(bool Value) { bIsInteractingWithWeapon = Value; }
	FORCEINLINE EWeaponStates GetWeaponState() const { return WeaponState; }
	FORCEINLINE void SetCanPerformNextAttack(bool Value) { bCanPerformNextAttack = Value; }

public:
	// Movmentments
	void SwordInteract();
	virtual void SwordAttack() override;
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

	FAttackData CurrentAttackData;
	EAttackType CurrentAttackType;

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
	UAnimMontage* MontageKick;

	FName NextAttackSectionName = ATTACK_DOWNSLASH;
	FName NextHitReactionSectionName = HIT_REACTION_DOWNSLASH;

	bool bIsSwordArmed = false;
	bool bIsInteractingWithWeapon = false;
	bool bIsLastBasicAttack = false;
	bool bIsQTEMode = false;

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
	void ExecuteBlock();
	void ExecuteKick();
	void ExecuteCounter();

private:
	UPlayerAnimInstance* PlayerAnimInstance;
	bool bEnemyCanBlockOrEvade;

};
