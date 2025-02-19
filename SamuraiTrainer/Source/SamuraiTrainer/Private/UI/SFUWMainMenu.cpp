// Copyright 2024 Sherwin Espela. All rights reserved.

#include "UI/SFUWMainMenu.h"
#include "UI/Buttons/UWButtonMainMenu.h"
#include "UI/Buttons/UWButtonNavigation.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"

void USFUWMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	TextDescription->SetText(FText::FromName(DESCRIPTION_BUTTON_PLAY));

	SetupButtonNavigationMapping();
	SubscribeToButtonEvents();
}

void USFUWMainMenu::SetupButtonNavigationMapping()
{
	// setup button navigation
	BMMPlay->SetBottomButton(BMMModes);
	BMMModes->SetTopButton(BMMPlay);
	BMMModes->SetBottomButton(BMMSettings);
	BMMSettings->SetTopButton(BMMModes);
	BMMSettings->SetBottomButton(BMMDevBio);
	BMMDevBio->SetTopButton(BMMSettings);
	BMMDevBio->SetBottomButton(BMMTutorial);
	BMMTutorial->SetTopButton(BMMDevBio);

	CurrentButton = BMMPlay;
}

void USFUWMainMenu::SubscribeToButtonEvents()
{
	BMMPlay->OnButtonSelectStarted.AddDynamic(this, &USFUWMainMenu::HandleButtonSelectStarted);
	BMMModes->OnButtonSelectStarted.AddDynamic(this, &USFUWMainMenu::HandleButtonSelectStarted);
	BMMSettings->OnButtonSelectStarted.AddDynamic(this, &USFUWMainMenu::HandleButtonSelectStarted);
	BMMDevBio->OnButtonSelectStarted.AddDynamic(this, &USFUWMainMenu::HandleButtonSelectStarted);
	BMMTutorial->OnButtonSelectStarted.AddDynamic(this, &USFUWMainMenu::HandleButtonSelectStarted);


	BMMPlay->OnButtonSelectAnimFinished.AddDynamic(this, &USFUWMainMenu::HandleButtonSelectAnimFinished);
	BMMModes->OnButtonSelectAnimFinished.AddDynamic(this, &USFUWMainMenu::HandleButtonSelectAnimFinished);
	BMMSettings->OnButtonSelectAnimFinished.AddDynamic(this, &USFUWMainMenu::HandleButtonSelectAnimFinished);
	BMMDevBio->OnButtonSelectAnimFinished.AddDynamic(this, &USFUWMainMenu::HandleButtonSelectAnimFinished);
	BMMTutorial->OnButtonSelectAnimFinished.AddDynamic(this, &USFUWMainMenu::HandleButtonSelectAnimFinished);
}

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

void USFUWMainMenu::HandleMainMenuEntryAnimFinished()
{
	OnMainMenuEntryAnimFinished.Broadcast();
}

void USFUWMainMenu::HandleButtonSelectStarted()
{
	bCanNavigateToNextButton = false;
	HBConfirmGroup->SetVisibility(ESlateVisibility::Hidden);
}

void USFUWMainMenu::HandleButtonSelectAnimFinished()
{
	bCanNavigateToNextButton = true;
	HBConfirmGroup->SetVisibility(ESlateVisibility::Visible);
}
