// Copyright 2024 Sherwin Espela. All rights reserved.


#include "UI/SFUWLevelIntro.h"

void USFUWLevelIntro::HandleLevelIntroAnimationCompleted()
{
	OnLevelIntroCompleted.Broadcast();
}
