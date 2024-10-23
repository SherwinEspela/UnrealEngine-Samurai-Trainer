// Copyright 2024 Sherwin Espela. All rights reserved.


#include "UI/SFUWMainMenu.h"

void USFUWMainMenu::PlayLogoExit()
{
	OnPlayLogoExit();
}

void USFUWMainMenu::PlayEnterMainMenu()
{
	OnPlayEnterMainMenu();
}

void USFUWMainMenu::HandleLogoIntroAnimFinished()
{
	OnLogoIntroAnimFinished.Broadcast();
}
