// Copyright 2024 Sherwin Espela. All rights reserved.


#include "UI/SFUWExitableMenu.h"

void USFUWExitableMenu::HandleExitGameAnimFinished()
{
	OnExitGameAnimFinished.Broadcast();
}
