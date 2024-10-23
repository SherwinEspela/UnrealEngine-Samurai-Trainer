// Copyright 2024 Sherwin Espela. All rights reserved.


#include "UI/Buttons/UWButtonNavigation.h"

UUWButtonNavigation* UUWButtonNavigation::SelectTopButton()
{
	if (BNTop) return BNTop;
	return nullptr;
}

UUWButtonNavigation* UUWButtonNavigation::SelectBottomButton()
{
	if (BNBottom) return BNBottom;
	return nullptr;
}
