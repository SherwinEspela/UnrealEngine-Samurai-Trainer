// Copyright 2024 Sherwin Espela. All rights reserved.


#include "UI/SFUWLevelMenu.h"
#include "UI/Buttons/UWButtonMainMenu.h"
#include "UI/Buttons/UWButtonNavigation.h"
#include "Components/TextBlock.h"

void USFUWLevelMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// setup button navigation
	BMMResume->SetBottomButton(BMMControls);
	BMMControls->SetTopButton(BMMResume);
	BMMControls->SetBottomButton(BMMSettings);
	BMMSettings->SetTopButton(BMMControls);
	BMMSettings->SetBottomButton(BMMExit);
	BMMExit->SetTopButton(BMMSettings);

	CurrentButton = BMMResume;
}

void USFUWLevelMenu::HandleDisplayLevelMenuCompleted()
{
	OnDisplayLevelMenuCompleted.Broadcast();
}

void USFUWLevelMenu::HandleHideLevelMenuCompleted()
{
	OnHideLevelMenuCompleted.Broadcast();
}
