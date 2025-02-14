// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerController/SFPlayerControllerBase.h"
#include "EnumLevelResult.h"
#include "GameFramework/PlayerController.h"
#include "STPlayerController.generated.h"

class ASTPlayerCharacter;
class UInputMappingContext;
class UInputAction;
class UEnhancedInputComponent;
class ADisplayLabelActor;
class USFUWLevelMenu;
class USFUWLevelIntro;
class USFUWLevelResults;
class USoundBase;
class UAudioComponent;
class USFSaveGameData;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelIntroHandledSignature);

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API ASTPlayerController : public ASFPlayerControllerBase
{
	GENERATED_BODY()

	

public:
	void HandleAllEnemiesKilled();

	FLevelIntroHandledSignature OnLevelIntroHandled;

	//static int ShowdownCounter;

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
	UPROPERTY(EditDefaultsOnly, Category = "Level UI")
	TSubclassOf<USFUWLevelMenu> SFUWLevelMenuClass;

	UPROPERTY(BlueprintReadOnly, Category = "Level UI")
	USFUWLevelMenu* LevelMenu;

	UPROPERTY(EditDefaultsOnly, Category = "Level UI")
	TSubclassOf<USFUWLevelIntro> SFUWLevelIntroClass;

	UPROPERTY(BlueprintReadOnly, Category = "Level UI")
	USFUWLevelIntro* LevelIntro;

	UPROPERTY(EditDefaultsOnly, Category = "Level UI")
	TSubclassOf<USFUWLevelResults> SFUWLevelResultsClass;

	UPROPERTY(BlueprintReadOnly, Category = "Level UI")
	USFUWLevelResults* LevelResults;

protected:
	// Sound FX
	UPROPERTY(EditDefaultsOnly, Category = "Level Music")
	TObjectPtr<USoundBase> SoundMusic;

	TObjectPtr<UAudioComponent> LevelMusicAudioComponent;

protected:
	void RestartLevel();
	void DisplayLevelMenu();
	void HideLevelMenu();
	void SelectTopButton();
	void SelectBottomButton();
	void ConfirmSelectedButton();
	void ToggleDebuggerDisplay();
	void LevelResultsEvent();

	UFUNCTION()
	void HandleDisplayLevelMenuCompleted();

	UFUNCTION()
	void HandleHideLevelMenuCompleted();

	UFUNCTION()
	void HandleExitMenuFinished();

	UFUNCTION()
	void HandleLevelIntroCompleted();

	UFUNCTION()
	void HandlePlayerDied();

protected:
	// Saving Game Data
	UPROPERTY()
	USFSaveGameData* SaveGameData;

private:
	void IncrementAndSaveShowdownCount();

private:
	TObjectPtr<ASTPlayerCharacter> PlayerCharacter;
	UEnhancedInputComponent* EnhancedInputComponent;
	ADisplayLabelActor* DisplayLabel;
	bool bIsDebuggerDisplayed = true;
	bool bIsLevelMenuDisplayed = false;
	bool bIsDisplayLevelMenuCompleted = false;
	bool bIsHideLevelMenuCompleted = true;
	bool bIsGameExiting = false;
	bool bLevelIntroCompleted = false;
	ELevelResultType LevelResultType;
};
