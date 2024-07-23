// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/STBaseCharacter.h"
#include "CustomEnums.h"
#include "STPlayerCharacter.generated.h"

#define ATTACK_SOCKET_FRONT FName("AttackSocketFront")
#define ATTACK_SOCKET_BACK FName("AttackSocketBack")

#define ATTACK_DOWNSLASH FName("Attack1")
#define ATTACK_UPSLASH FName("Attack2")
#define KICK1 FName("Kick1")
#define KICK2 FName("Kick2")

#define HIT_REACTION_DOWNSLASH FName("HitReaction1")
#define HIT_REACTION_UPSLASH FName("HitReaction2")
#define HR_BEHIND1 FName("HRBehind1")
#define HR_BEHIND2 FName("HRBehind2")
#define HIT_REACTION_KICK1 FName("HRKick1")
#define HIT_REACTION_KICK2 FName("HRKick2")
#define HIT_REACTION_CE1 FName("HRComboEnd1")
#define HIT_REACTION_CE2 FName("HRComboEnd2")
#define HR_KICK_CE1 FName("HRKickComboEnd1")
#define HR_COUNTER_CE1 FName("HRCounterComboEnd1")
#define HR_BACK_CE1 FName("HRBackComboEnd1")
#define HR_COUNTER1 FName("HRCounter1")
#define HR_COUNTER2 FName("HRCounter2")
#define HR_COUNTER_BLANK FName("CounterBlank")

#define SWORD_ATTACK_COMBO_END1 FName("ComboEnd1")
#define SWORD_ATTACK_COMBO_END2 FName("ComboEnd2")
#define SA_BACK_CE1 FName("BackComboEnd1")
#define KICK_COMBO_END1 FName("KickComboEnd1")
#define COUNTER_COMBO_END1 FName("CounterComboEnd1")

#define ATTACK_COUNTER1 FName("Counter1")
#define ATTACK_COUNTER2 FName("Counter2")
#define ATTACK_COUNTER3 FName("Counter3")
#define ATTACK_COUNTER4 FName("Counter4")
#define ATTACK_COUNTER5 FName("Counter5")

class AKatana;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class USkeletalMeshComponent;
class ASTEnemyCharacter;
class UCapsuleComponent;

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
	FName AttackSocketName = ATTACK_SOCKET_FRONT;
};

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

	void HandleBasicAttackCompleted();

	UFUNCTION(BlueprintCallable)
	ASTEnemyCharacter* GetTargetLockedEnemy() const;

	UFUNCTION(BlueprintCallable)
	FName GetAttackSocketName() const;

public:
	FORCEINLINE void SetIsInteractingWithWeapon(bool Value) { bIsInteractingWithWeapon = Value; }
	FORCEINLINE EWeaponStates GetWeaponState() const { return WeaponState; }
	FORCEINLINE void SetMovementState(EMovementStates Value) { MovementState = Value; }
	FORCEINLINE void SetCanPerformNextAttack(bool Value) { bCanPerformNextAttack = Value; }

protected:
	virtual void BeginPlay() override;

	void AttachSwordToSocket(FName SocketName);
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

	UPROPERTY(BlueprintReadOnly, Category = "Movement States")
	EMovementStates MovementState = EMovementStates::EPMS_Default;

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

private:
	UPROPERTY(EditAnywhere, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AKatana> KatanaClass;
	AKatana* Katana;
	UAnimInstance* PlayerAnimInstance;
	FName CurrentAttackSocketName;

};
