// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/STBaseCharacter.h"
#include "CustomEnums.h"
#include "STPlayerCharacter.generated.h"

class AKatana;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class USkeletalMeshComponent;

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

public:
	FORCEINLINE void SetIsInteractingWithWeapon(bool Value) { bIsInteractingWithWeapon = Value; }
	FORCEINLINE EWeaponStates GetWeaponState() const { return WeaponState; }
	FORCEINLINE void SetMovementState(EMovementStates Value) { MovementState = Value; }

protected:
	virtual void BeginPlay() override;

	void AttachSwordToSocket(FName SocketName);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Camera Setup")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category = "Camera Setup")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(BlueprintReadOnly, Category = "Movement States")
	EMovementStates MovementState = EMovementStates::EPMS_Default;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon States")
	EWeaponStates WeaponState = EWeaponStates::EWS_Stored;

protected:
	// Animations
	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageSwordInteract;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageAttack;

	bool bIsSwordArmed = false;
	bool bIsInteractingWithWeapon = false;

private:
	UPROPERTY(EditAnywhere, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AKatana> KatanaClass;

	AKatana* Katana;
};
