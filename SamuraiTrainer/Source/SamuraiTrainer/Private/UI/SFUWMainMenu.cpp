// Copyright 2024 Sherwin Espela. All rights reserved.


#include "UI/SFUWMainMenu.h"
#include "UI/Buttons/UWButtonMainMenu.h"
#include "UI/Buttons/UWButtonNavigation.h"

void USFUWMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

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
