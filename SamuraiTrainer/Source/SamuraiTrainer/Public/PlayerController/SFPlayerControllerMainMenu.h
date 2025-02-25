// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerController/SFPlayerControllerBase.h"
#include "GameFramework/PlayerController.h"
#include "SFPlayerControllerMainMenu.generated.h"

class USFUWMainMenu;
class UInputMappingContext;
class UInputAction;
class USFSaveGameData;

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API ASFPlayerControllerMainMenu : public ASFPlayerControllerBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlaySequenceWhenMainMenuEntered();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlaySequenceAtPlayButtonSelected();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlaySequenceAtModesButtonSelected();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlaySequenceAtSettingsButtonSelected();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlaySequenceAtDevBioButtonSelected();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlaySequenceAtTutorialsButtonSelected();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayButtonClicked();

	UFUNCTION(BlueprintCallable)
	void HandleMainMenuFadeOutCompleted();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USFUWMainMenu> SFUWMainMenuClass;

	UPROPERTY(BlueprintReadOnly, Category = "Main Menu UI")
	USFUWMainMenu* UWMainMenu;

protected:
	// Player Inputs
	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	UInputMappingContext* IMCMainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* IAEnterMainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* IARestartLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* IASelectTopButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* IASelectBottomButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* IAPlayButton;

protected:
	void EnterMainMenu();
	void RestartLevel();
	void SelectTopButton();
	void SelectBottomButton();
	void PlayButtonClicked();
	void HandleButtonSelected(EMainMenuButtonTypes ButtonType) override;

protected:
	// Saving Game Data
	UPROPERTY()
	USFSaveGameData* SaveGameData;

private:
	// Event Handlers
	UFUNCTION()
	void HandleLogoIntroAnimFinished();

	UFUNCTION()
	void HandleMainMenuEntryAnimFinished();

private:
	UEnhancedInputComponent* EnhancedInputComponent;
	bool bLogoTitleEntered = false;
	bool bMainMenuEntered = false;
	bool bIsPlayModeSelected = false;
};
