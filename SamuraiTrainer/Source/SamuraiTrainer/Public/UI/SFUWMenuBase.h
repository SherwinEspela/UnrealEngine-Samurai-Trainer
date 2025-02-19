// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#define DESCRIPTION_BUTTON_PLAY FName("Unleash Your Fury! Embark on a Journey of Revenge.")
#define DESCRIPTION_BUTTON_MODES FName("Select a mode to begin your journey. Write your own legend. [Unavailable]")
#define DESCRIPTION_BUTTON_SETTINGS FName("Configure game settings to suit your preferences. [Unavailable]")
#define DESCRIPTION_BUTTON_DEVBIO FName("Meet Sherwin Espela, Creator of Samurai Fury. [Unavailable]")
#define DESCRIPTION_BUTTON_TUTORIALS FName("Understand the intricacies of Samurai Fury's gameplay. [Unavailable]")
#define DESCRIPTION_BUTTON_LEVEL_RESUME FName("Resume playing the game.")
#define DESCRIPTION_BUTTON_LEVEL_CONTINUE FName("Continue playing to next level.")
#define DESCRIPTION_BUTTON_LEVEL_RESTART FName("Restart the current level.")
#define DESCRIPTION_BUTTON_LEVEL_CONTROLS FName("Show the control configurations. [Unavailable]")
#define DESCRIPTION_BUTTON_LEVEL_EXIT FName("Stop playing the game and go to main menu.")

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SFUWMenuBase.generated.h"

class UUWButtonMainMenu;
class UUWButtonNavigation;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FButtonSelectedSignature, EMainMenuButtonTypes, ButtonType);

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API USFUWMenuBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void SelectTopButton();
	virtual void SelectBottomButton();

public:
	FButtonSelectedSignature OnButtonSelected;

protected:
	void SetDescriptionForSelectedButtonType(EMainMenuButtonTypes Value);
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TextDescription;
	UUWButtonMainMenu* CurrentButton;

	bool bCanNavigateToNextButton = true;

private:
	void NavigateToNextButton(UUWButtonNavigation* Value);
	
};
