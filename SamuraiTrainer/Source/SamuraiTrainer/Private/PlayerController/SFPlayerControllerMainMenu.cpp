// Copyright 2024 Sherwin Espela. All rights reserved.


#include "PlayerController/SFPlayerControllerMainMenu.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "UI/SFUWMainMenu.h"
#include "Kismet/GameplayStatics.h"

void ASFPlayerControllerMainMenu::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	PlayerSubsystem->AddMappingContext(IMCMainMenu, 0);

	bLogoTitleEntered = false;
	bMainMenuEntered = false;

	if (SFUWMainMenuClass)
	{
		UWMainMenu = CreateWidget<USFUWMainMenu>(GetWorld(), SFUWMainMenuClass);
		UWMainMenu->AddToViewport();
		UWMainMenu->OnLogoIntroAnimFinished.AddDynamic(this, &ASFPlayerControllerMainMenu::HandleLogoIntroAnimFinished);
	}
}

void ASFPlayerControllerMainMenu::SetupInputComponent()
{
	Super::SetupInputComponent();

	EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(IAEnterMainMenu, ETriggerEvent::Triggered, this, &ASFPlayerControllerMainMenu::EnterMainMenu);
	EnhancedInputComponent->BindAction(IARestartLevel, ETriggerEvent::Triggered, this, &ASFPlayerControllerMainMenu::RestartLevel);
}

void ASFPlayerControllerMainMenu::EnterMainMenu()
{
	if (bMainMenuEntered) return;
	if (!bLogoTitleEntered) return;
	if (!UWMainMenu) return;
	UWMainMenu->PlayLogoExit();
	bMainMenuEntered = true;
}

void ASFPlayerControllerMainMenu::RestartLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void ASFPlayerControllerMainMenu::HandleLogoIntroAnimFinished()
{
	bLogoTitleEntered = true;
}
