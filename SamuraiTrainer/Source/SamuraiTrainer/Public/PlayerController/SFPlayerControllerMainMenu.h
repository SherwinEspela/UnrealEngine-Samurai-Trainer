// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomEnums.h"
#include "GameFramework/PlayerController.h"
#include "SFPlayerControllerMainMenu.generated.h"

class USFUWMainMenu;
class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API ASFPlayerControllerMainMenu : public APlayerController
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

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USFUWMainMenu> SFUWMainMenuClass;

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

protected:
	void EnterMainMenu();
	void RestartLevel();
	void SelectTopButton();
	void SelectBottomButton();

private:
	// Event Handlers
	UFUNCTION()
	void HandleLogoIntroAnimFinished();

	UFUNCTION()
	void HandleMainMenuEntryAnimFinished();

	UFUNCTION()
	void HandleButtonSelected(EMainMenuButtonTypes ButtonType);

private:
	UEnhancedInputComponent* EnhancedInputComponent;
	bool bLogoTitleEntered = false;
	bool bMainMenuEntered = false;
};
