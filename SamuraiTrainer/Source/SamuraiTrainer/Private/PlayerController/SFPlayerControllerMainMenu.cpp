// Copyright 2024 Sherwin Espela. All rights reserved.


#include "PlayerController/SFPlayerControllerMainMenu.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "UI/SFUWMainMenu.h"
#include "DataPersistence/SFSaveGameData.h"
#include "Utility/RandomLevelLoader.h"
#include "Kismet/GameplayStatics.h"

void ASFPlayerControllerMainMenu::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	PlayerSubsystem->AddMappingContext(IMCMainMenu, 0);

	bLogoTitleEntered = false;
	bMainMenuEntered = false;
	CurrentSelectedButtonType = EMainMenuButtonTypes::EMMBT_Default;
	RandomLevelLoader::MaxLevelCount = 2;

	if (SaveGameData == nullptr)
	{
		// Create save game data
		SaveGameData = CastChecked<USFSaveGameData>(UGameplayStatics::CreateSaveGameObject(USFSaveGameData::StaticClass()));
	}

	if (SaveGameData)
	{
		FString SaveSlotName = SaveGameData->SaveSlotName;
		uint32 UserIndex = SaveGameData->UserIndex;

		try
		{
			SaveGameData->ShowdownCounter = 1;
			UGameplayStatics::SaveGameToSlot(SaveGameData, SaveSlotName, UserIndex);
		}
		catch (const std::exception&) {}
	}

	if (SFUWMainMenuClass)
	{
		UWMainMenu = CreateWidget<USFUWMainMenu>(GetWorld(), SFUWMainMenuClass);
		UWMainMenu->AddToViewport();
		UWMainMenu->OnLogoIntroAnimFinished.AddDynamic(this, &ASFPlayerControllerMainMenu::HandleLogoIntroAnimFinished);
		UWMainMenu->OnMainMenuEntryAnimFinished.AddDynamic(this, &ASFPlayerControllerMainMenu::HandleMainMenuEntryAnimFinished);
		UWMainMenu->OnButtonSelected.AddDynamic(this, &ASFPlayerControllerMainMenu::HandleButtonSelected);
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
	EnhancedInputComponent->BindAction(IAPlayButton, ETriggerEvent::Triggered, this, &ASFPlayerControllerMainMenu::PlayButtonClicked);
}

void ASFPlayerControllerMainMenu::EnterMainMenu()
{
	if (bMainMenuEntered) return;
	if (!bLogoTitleEntered) return;
	if (!UWMainMenu) return;

	CurrentSelectedButtonType = EMainMenuButtonTypes::EMMBT_Play;
	UWMainMenu->PlayLogoExit();
	OnPlaySequenceWhenMainMenuEntered();
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

void ASFPlayerControllerMainMenu::PlayButtonClicked()
{
	if (CurrentSelectedButtonType != EMainMenuButtonTypes::EMMBT_Play) return;
	if (!bMainMenuEntered) return;

	bIsPlayButtonClicked = true;
	OnPlayButtonClicked();
}

void ASFPlayerControllerMainMenu::HandleLogoIntroAnimFinished()
{
	bLogoTitleEntered = true;
}

void ASFPlayerControllerMainMenu::HandleMainMenuEntryAnimFinished()
{
	bMainMenuEntered = true;
}

void ASFPlayerControllerMainMenu::HandleButtonSelected(EMainMenuButtonTypes ButtonType)
{
	Super::HandleButtonSelected(ButtonType);

	switch (ButtonType)
	{
	case EMainMenuButtonTypes::EMMBT_Play:
		OnPlaySequenceAtPlayButtonSelected();
		break;
	case EMainMenuButtonTypes::EMMBT_Modes:
		OnPlaySequenceAtModesButtonSelected();
		break;
	case EMainMenuButtonTypes::EMMBT_Settings:
		OnPlaySequenceAtSettingsButtonSelected();
		break;
	case EMainMenuButtonTypes::EMMBT_DevBio:
		OnPlaySequenceAtDevBioButtonSelected();
		break;
	case EMainMenuButtonTypes::EMMBT_Tutorials:
		OnPlaySequenceAtTutorialsButtonSelected();
		break;
	default:
		break;
	}
}

void ASFPlayerControllerMainMenu::HandleMainMenuFadeOutCompleted()
{
	RandomLevelLoader::LoadRandomLevel(this);
}
