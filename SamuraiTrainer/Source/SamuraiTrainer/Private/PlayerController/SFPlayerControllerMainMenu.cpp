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
		UWMainMenu->OnMainMenuEntryAnimFinished.AddDynamic(this, &ASFPlayerControllerMainMenu::HandleMainMenuEntryAnimFinished);
	}
}

void ASFPlayerControllerMainMenu::SetupInputComponent()
{
	Super::SetupInputComponent();

	EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(IAEnterMainMenu, ETriggerEvent::Triggered, this, &ASFPlayerControllerMainMenu::EnterMainMenu);
	EnhancedInputComponent->BindAction(IARestartLevel, ETriggerEvent::Triggered, this, &ASFPlayerControllerMainMenu::RestartLevel);
	EnhancedInputComponent->BindAction(IASelectTopButton, ETriggerEvent::Triggered, this, &ASFPlayerControllerMainMenu::SelectTopButton);
	EnhancedInputComponent->BindAction(IASelectBottomButton, ETriggerEvent::Triggered, this, &ASFPlayerControllerMainMenu::SelectBottomButton);
}

void ASFPlayerControllerMainMenu::EnterMainMenu()
{
	if (bMainMenuEntered) return;
	if (!bLogoTitleEntered) return;
	if (!UWMainMenu) return;
	UWMainMenu->PlayLogoExit();
}

void ASFPlayerControllerMainMenu::RestartLevel()
{
	if (!bMainMenuEntered) return;
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void ASFPlayerControllerMainMenu::SelectTopButton()
{
	if (!bMainMenuEntered) return;
	UWMainMenu->SelectTopButton();
}

void ASFPlayerControllerMainMenu::SelectBottomButton()
{
	if (!bMainMenuEntered) return;
	UWMainMenu->SelectBottomButton();
}

void ASFPlayerControllerMainMenu::HandleLogoIntroAnimFinished()
{
	bLogoTitleEntered = true;
}

void ASFPlayerControllerMainMenu::HandleMainMenuEntryAnimFinished()
{
	bMainMenuEntered = true;
}
