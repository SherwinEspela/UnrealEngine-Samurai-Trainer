// Copyright 2024 Sherwin Espela. All rights reserved.


#include "UI/SFUWLevelResults.h"
#include "UI/Buttons/UWButtonMainMenu.h"
#include "UI/Buttons/UWButtonNavigation.h"

void USFUWLevelResults::NativeConstruct()
{
	Super::NativeConstruct();

	BMMContinue->SetBottomButton(BMMExit);
	BMMExit->SetTopButton(BMMContinue);
	CurrentButton = BMMContinue;
}

void USFUWLevelResults::SetupButtonsByLevelResult(ELevelResultType LevelResult)
{
	switch (LevelResult)
	{
	case ELevelResultType::ELRT_Completed:
		break;
	case ELevelResultType::ELRT_PlayerDied:
		break;
	default:
		break;
	}
}
