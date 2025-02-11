// Copyright 2024 Sherwin Espela. All rights reserved.


#include "UI/SFUWLevelResults.h"
#include "UI/Buttons/UWButtonMainMenu.h"
#include "Components/TextBlock.h"
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
		BMMContinue->SetTitle("CONTINUE");
		BMMContinue->SetButtonType(EMainMenuButtonTypes::EMMBT_LevelContinue);
		SetDescriptionForSelectedButtonType(EMainMenuButtonTypes::EMMBT_LevelContinue);
		break;
	case ELevelResultType::ELRT_PlayerDied:
		BMMContinue->SetTitle("RESTART");
		BMMContinue->SetButtonType(EMainMenuButtonTypes::EMMBT_LevelRestart);
		SetDescriptionForSelectedButtonType(EMainMenuButtonTypes::EMMBT_LevelRestart);
		break;
	default:
		break;
	}
}
