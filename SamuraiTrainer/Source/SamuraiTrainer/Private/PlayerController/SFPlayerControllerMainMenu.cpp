// Copyright 2024 Sherwin Espela. All rights reserved.


#include "PlayerController/SFPlayerControllerMainMenu.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "UI/SFUWMainMenu.h"

void ASFPlayerControllerMainMenu::BeginPlay()
{
	Super::BeginPlay();

	if (SFUWMainMenuClass)
	{
		UWMainMenu = CreateWidget<USFUWMainMenu>(GetWorld(), SFUWMainMenuClass);
		UWMainMenu->AddToViewport();
	}
}
