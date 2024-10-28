// Copyright 2024 Sherwin Espela. All rights reserved.

#define DESCRIPTION_BUTTON_PLAY FName("Unleash Your Fury! Embark on a Journey of Revenge.")
#define DESCRIPTION_BUTTON_MODES FName("Select a mode to begin your journey. Write your own legend.")
#define DESCRIPTION_BUTTON_SETTINGS FName("Configure game settings to suit your preferences.")
#define DESCRIPTION_BUTTON_DEVBIO FName("Meet Sherwin Espela, Creator of Samurai Fury.")
#define DESCRIPTION_BUTTON_TUTORIALS FName("Understand the intricacies of Samurai Fury's gameplay.")

#include "UI/SFUWMainMenu.h"
#include "UI/Buttons/UWButtonMainMenu.h"
#include "UI/Buttons/UWButtonNavigation.h"
#include "Components/TextBlock.h"

void USFUWMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	TextDescription->SetText(FText::FromName(DESCRIPTION_BUTTON_PLAY));

	// setup button navigation
	BMMPlay->SetBottomButton(BMMModes);
	BMMModes->SetTopButton(BMMPlay);
	BMMModes->SetBottomButton(BMMSettings);
	BMMSettings->SetTopButton(BMMModes);
	BMMSettings->SetBottomButton(BMMDevBio);
	BMMDevBio->SetTopButton(BMMSettings);
	BMMDevBio->SetBottomButton(BMMTutorial);
	BMMTutorial->SetTopButton(BMMDevBio);

	CurrentMMButton = BMMPlay;
}

void USFUWMainMenu::SetupButtonNavigationMapping()
{
}

void USFUWMainMenu::SubscribeToButtonEvents()
{
}

void USFUWMainMenu::PlayLogoExit()
{
	OnPlayLogoExit();
}

void USFUWMainMenu::PlayEnterMainMenu()
{
	OnPlayEnterMainMenu();
}

void USFUWMainMenu::SelectTopButton()
{
	NavigateToNextButton(CurrentMMButton->SelectTopButton());
}

void USFUWMainMenu::SelectBottomButton()
{
	NavigateToNextButton(CurrentMMButton->SelectBottomButton());
}

void USFUWMainMenu::NavigateToNextButton(UUWButtonNavigation* Value)
{
	if (Value)
	{
		CurrentMMButton->PlayUnselect();
		auto NextButton = Cast<UUWButtonMainMenu>(Value);
		NextButton->PlaySelect();
		CurrentMMButton = NextButton;

		EMainMenuButtonTypes ButtonType = CurrentMMButton->GetButtonType();
		OnButtonSelected.Broadcast(ButtonType);
		SetDescriptionForSelectedButtonType(ButtonType);
	}
}

void USFUWMainMenu::SetDescriptionForSelectedButtonType(EMainMenuButtonTypes Value)
{
	switch (Value)
	{
	case EMainMenuButtonTypes::EMMBT_Play:
		TextDescription->SetText(FText::FromName(DESCRIPTION_BUTTON_PLAY));
		break;
	case EMainMenuButtonTypes::EMMBT_Modes:
		TextDescription->SetText(FText::FromName(DESCRIPTION_BUTTON_MODES));
		break;
	case EMainMenuButtonTypes::EMMBT_Settings:
		TextDescription->SetText(FText::FromName(DESCRIPTION_BUTTON_SETTINGS));
		break;
	case EMainMenuButtonTypes::EMMBT_DevBio:
		TextDescription->SetText(FText::FromName(DESCRIPTION_BUTTON_DEVBIO));
		break;
	case EMainMenuButtonTypes::EMMBT_Tutorials:
		TextDescription->SetText(FText::FromName(DESCRIPTION_BUTTON_TUTORIALS));
		break;
	default:
		break;
	}
}

void USFUWMainMenu::HandleLogoIntroAnimFinished()
{
	OnLogoIntroAnimFinished.Broadcast();
}

void USFUWMainMenu::HandleMainMenuEntryAnimFinished()
{
	OnMainMenuEntryAnimFinished.Broadcast();
}
