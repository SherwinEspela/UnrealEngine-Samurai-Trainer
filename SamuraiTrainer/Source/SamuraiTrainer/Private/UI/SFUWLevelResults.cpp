// Copyright 2024 Sherwin Espela. All rights reserved.


#include "UI/SFUWLevelResults.h"
#include "UI/Buttons/UWButtonMainMenu.h"
#include "UI/Buttons/UWButtonNavigation.h"

void USFUWLevelResults::NativeConstruct()
{
	Super::NativeConstruct();

	// setup button navigation
	BMMContinue->SetBottomButton(BMMExit);
	BMMExit->SetTopButton(BMMContinue);

	CurrentButton = BMMContinue;
}
