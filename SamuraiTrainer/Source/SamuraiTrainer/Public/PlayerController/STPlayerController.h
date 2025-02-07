// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerController/SFPlayerControllerBase.h"
#include "GameFramework/PlayerController.h"
#include "STPlayerController.generated.h"

class ASTPlayerCharacter;
class UInputMappingContext;
class UInputAction;
class UEnhancedInputComponent;
class ADisplayLabelActor;
class USFUWLevelMenu;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API ASTPlayerController : public ASFPlayerControllerBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void SwordInteract();
	void Attack();
	void AttackCombo2();
	void Block();
	void ParryOrBlock();
	void Kick();

protected:
	// Player Inputs
	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* InputActionSwordInteract;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* InputActionAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* InputActionAttackCombo2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* InputActionBlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* InputActionKick;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* InputActionCounter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* InputActionRestartLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* InputActionToggleDebuggerDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* IADpadUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* IADpadDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* IALevelMenu;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* IAButtonA;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USFUWLevelMenu> SFUWLevelMenuClass;

	UPROPERTY(BlueprintReadOnly, Category = "Level Menu UI")
	USFUWLevelMenu* LevelMenu;

protected:
	void RestartLevel();
	void DisplayLevelMenu();
	void HideLevelMenu();
	void SelectTopButton();
	void SelectBottomButton();
	void ConfirmSelectedButton();
	void ToggleDebuggerDisplay();

private:
	TObjectPtr<ASTPlayerCharacter> PlayerCharacter;
	UEnhancedInputComponent* EnhancedInputComponent;
	ADisplayLabelActor* DisplayLabel;
	bool bIsDebuggerDisplayed = true;
	bool bIsLevelMenuDisplayed = false;
};
