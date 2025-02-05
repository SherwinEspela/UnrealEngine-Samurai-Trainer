// Copyright 2024 Sherwin Espela. All rights reserved.

#include "UI/SFUWMenuBase.h"
#include "UI/Buttons/UWButtonMainMenu.h"
#include "UI/Buttons/UWButtonNavigation.h"
#include "Components/TextBlock.h"

void USFUWMenuBase::SelectTopButton()
{
	NavigateToNextButton(CurrentButton->SelectTopButton());
}

void USFUWMenuBase::SelectBottomButton()
{
	NavigateToNextButton(CurrentButton->SelectBottomButton());
}

void USFUWMenuBase::NavigateToNextButton(UUWButtonNavigation* Value)
{
	if (Value)
	{
		CurrentButton->PlayUnselect();
		auto NextButton = Cast<UUWButtonMainMenu>(Value);
		NextButton->PlaySelect();
		CurrentButton = NextButton;

		EMainMenuButtonTypes ButtonType = CurrentButton->GetButtonType();
		OnButtonSelected.Broadcast(ButtonType);
		SetDescriptionForSelectedButtonType(ButtonType);
	}
}

void USFUWMenuBase::SetDescriptionForSelectedButtonType(EMainMenuButtonTypes Value)
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
