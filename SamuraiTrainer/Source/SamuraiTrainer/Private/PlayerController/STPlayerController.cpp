// Copyright 2024 Sherwin Espela. All rights reserved.


#include "PlayerController/STPlayerController.h"
#include "Character/STPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DisplayLabelActor.h"
#include "Combat/TargetLockActor.h"
#include "UI/SFUWLevelMenu.h"
#include "UI/SFUWLevelIntro.h"
#include "UI/SFUWLevelResults.h"
#include "DataPersistence/SFSaveGameData.h"
#include "Utility/RandomLevelLoader.h"
#include "Components/AudioComponent.h"

#define MAIN_MENU_MAP FName("MainMenuMap")
#define LEVEL1_MAP FName("SetupMap2_OneEnemy")
#define MUSIC_VOLUME_MAX 0.75f
#define MUSIC_VOLUME_MIN 0.3f

void ASTPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ASTPlayerCharacter>(GetPawn());
	PlayerCharacter->OnCharacterDied.AddDynamic(this, &ASTPlayerController::HandlePlayerDied);

	UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	PlayerSubsystem->AddMappingContext(InputMappingContext, 0);

	TArray<AActor*> DisplayLabelActors;
	UGameplayStatics::GetAllActorsWithTag(this, FName("DisplayLabel"), DisplayLabelActors);
	if (DisplayLabelActors.Num() > 0)
	{
		DisplayLabel = Cast<ADisplayLabelActor>(DisplayLabelActors[0]);
	}

	if (SaveGameData == nullptr)
	{
		// Create save game data
		SaveGameData = CastChecked<USFSaveGameData>(UGameplayStatics::CreateSaveGameObject(USFSaveGameData::StaticClass()));
	}

	if (SFUWLevelIntroClass)
	{
		LevelIntro = CreateWidget<USFUWLevelIntro>(GetWorld(), SFUWLevelIntroClass);
		LevelIntro->AddToViewport();

		if (SaveGameData)
		{
			FString SaveSlotName = SaveGameData->SaveSlotName;
			uint32 UserIndex = SaveGameData->UserIndex;
			if ((SaveGameData = Cast<USFSaveGameData>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex))))
			{
				LevelIntro->SetShowdownCount(SaveGameData->ShowdownCounter);
			}
		}

		LevelIntro->OnLevelIntroCompleted.AddDynamic(this, &ASTPlayerController::HandleLevelIntroCompleted);
	}

	bIsLevelMenuDisplayed = false;
	LevelResultType = ELevelResultType::EDPT_Default;
	LevelLoader = new RandomLevelLoader();
}

void ASTPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASTPlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASTPlayerController::Look);
	EnhancedInputComponent->BindAction(InputActionSwordInteract, ETriggerEvent::Triggered, this, &ASTPlayerController::SwordInteract);
	EnhancedInputComponent->BindAction(InputActionAttack, ETriggerEvent::Triggered, this, &ASTPlayerController::Attack);
	EnhancedInputComponent->BindAction(InputActionAttackCombo2, ETriggerEvent::Triggered, this, &ASTPlayerController::AttackCombo2);
	EnhancedInputComponent->BindAction(InputActionBlock, ETriggerEvent::Triggered, this, &ASTPlayerController::ParryOrBlock);
	EnhancedInputComponent->BindAction(InputActionEvade, ETriggerEvent::Triggered, this, &ASTPlayerController::Evade);
	EnhancedInputComponent->BindAction(InputActionRestartLevel, ETriggerEvent::Triggered, this, &ASTPlayerController::RestartLevel);
	EnhancedInputComponent->BindAction(InputActionToggleDebuggerDisplay, ETriggerEvent::Triggered, this, &ASTPlayerController::ToggleDebuggerDisplay);
	EnhancedInputComponent->BindAction(IADpadUp, ETriggerEvent::Triggered, this, &ASTPlayerController::SelectTopButton);
	EnhancedInputComponent->BindAction(IADpadDown, ETriggerEvent::Triggered, this, &ASTPlayerController::SelectBottomButton);
	EnhancedInputComponent->BindAction(IALevelMenu, ETriggerEvent::Triggered, this, &ASTPlayerController::DisplayLevelMenu);
	EnhancedInputComponent->BindAction(IAButtonA, ETriggerEvent::Triggered, this, &ASTPlayerController::ConfirmSelectedButton);

	IADpadUp->bTriggerWhenPaused = true;
	IADpadDown->bTriggerWhenPaused = true;
	IAButtonA->bTriggerWhenPaused = true;
}

void ASTPlayerController::Move(const FInputActionValue& Value)
{
	if (LevelResultType != ELevelResultType::EDPT_Default) return;
	if (!bLevelIntroCompleted) return;

	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	PlayerCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
	PlayerCharacter->AddMovementInput(RightDirection, MovementVector.X);
	PlayerCharacter->GetTargetLockActor()->SetLineEndVectors(ForwardDirection * MovementVector.Y, RightDirection * MovementVector.X);
}

void ASTPlayerController::Look(const FInputActionValue& Value)
{
	if (!bLevelIntroCompleted) return;

	FVector2D LookAxisVector = Value.Get<FVector2D>();
	PlayerCharacter->AddControllerYawInput(LookAxisVector.X);
	PlayerCharacter->AddControllerPitchInput(LookAxisVector.Y);
}

void ASTPlayerController::SwordInteract()
{
	if (LevelResultType != ELevelResultType::EDPT_Default) return;
	if (!bLevelIntroCompleted) return;

	PlayerCharacter->SwordInteract();
}

void ASTPlayerController::Attack()
{
	if (LevelResultType != ELevelResultType::EDPT_Default) return;
	if (!bLevelIntroCompleted) return;

	PlayerCharacter->SwordAttack();
}

void ASTPlayerController::AttackCombo2()
{
	if (LevelResultType != ELevelResultType::EDPT_Default) return;
	if (!bLevelIntroCompleted) return;

	PlayerCharacter->SwordAttackCombo2();
}

void ASTPlayerController::Block()
{
	if (LevelResultType != ELevelResultType::EDPT_Default) return;
	if (!bLevelIntroCompleted) return;

	PlayerCharacter->Block();
}

void ASTPlayerController::ParryOrBlock()
{
	if (LevelResultType != ELevelResultType::EDPT_Default) return;
	if (!bLevelIntroCompleted) return;

	PlayerCharacter->ParryOrBlock();
}

void ASTPlayerController::Evade()
{
	if (LevelResultType != ELevelResultType::EDPT_Default) return;
	if (!bLevelIntroCompleted) return;

	PlayerCharacter->Evade();
}

void ASTPlayerController::Kick()
{
	if (LevelResultType != ELevelResultType::EDPT_Default) return;
	if (!bLevelIntroCompleted) return;

	PlayerCharacter->Kick();
}

void ASTPlayerController::RestartLevel()
{
	if (!bLevelIntroCompleted) return;

	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void ASTPlayerController::DisplayLevelMenu()
{
	if (LevelResultType != ELevelResultType::EDPT_Default) return;
	if (!bLevelIntroCompleted) return;
	if (bIsLevelMenuDisplayed) return;
	if (!bIsHideLevelMenuCompleted) return;
	if (bIsGameExiting) return;
	
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	CurrentSelectedButtonType = EMainMenuButtonTypes::EMMBT_LevelResume;
	LevelMenu->OnDisplay();
	bIsLevelMenuDisplayed = true;
	bIsDisplayLevelMenuCompleted = false;
	bIsHideLevelMenuCompleted = false;
	LevelMusicAudioComponent->SetVolumeMultiplier(MUSIC_VOLUME_MIN);
	CurrentVolumeLevel = MUSIC_VOLUME_MIN;
}

void ASTPlayerController::HideLevelMenu()
{
	if (!bIsLevelMenuDisplayed) return;
	if (bIsGameExiting) return;

	UGameplayStatics::SetGamePaused(GetWorld(), false);
	LevelMenu->OnHide();
	LevelMusicAudioComponent->SetVolumeMultiplier(MUSIC_VOLUME_MAX);
	CurrentVolumeLevel = MUSIC_VOLUME_MAX;
}

void ASTPlayerController::SelectTopButton()
{
	if (LevelResultType != ELevelResultType::EDPT_Default && !bIsGameExiting)
	{
		LevelResults->SelectTopButton();	
		return;
	}

	if (!bIsLevelMenuDisplayed) return;
	if (!bIsDisplayLevelMenuCompleted) return;
	if (bIsGameExiting) return;

	LevelMenu->SelectTopButton();
}

void ASTPlayerController::SelectBottomButton()
{
	if (LevelResultType != ELevelResultType::EDPT_Default && !bIsGameExiting)
	{
		LevelResults->SelectBottomButton();
		return;
	}

	if (!bIsLevelMenuDisplayed) return;
	if (!bIsDisplayLevelMenuCompleted) return;
	if (bIsGameExiting) return;

	LevelMenu->SelectBottomButton();
}

void ASTPlayerController::ConfirmSelectedButton()
{
	if (LevelResultType != ELevelResultType::EDPT_Default && !bIsGameExiting)
	{
		switch (CurrentSelectedButtonType)
		{
		case EMainMenuButtonTypes::EMMBT_LevelContinue:
			LevelResults->OnExitMenu();
			break;
		case EMainMenuButtonTypes::EMMBT_LevelRestart:
			LevelResults->OnExitMenu();
			break;
		case EMainMenuButtonTypes::EMMBT_LevelExit:
			LevelResults->OnExitMenu();
			break;
		default:
			break;
		}

		return;
	}

	if (!bIsLevelMenuDisplayed) return;
	if (!bIsDisplayLevelMenuCompleted) return;

	switch (CurrentSelectedButtonType)
	{
	case EMainMenuButtonTypes::EMMBT_LevelResume:
		HideLevelMenu();
		break;
	case EMainMenuButtonTypes::EMMBT_LevelControls:
		break;
	case EMainMenuButtonTypes::EMMBT_LevelSettings:
		break;
	case EMainMenuButtonTypes::EMMBT_LevelExit:
		LevelMenu->OnExitMenu();
		break;
	case EMainMenuButtonTypes::EMMBT_Default:
		break;
	default:
		break;
	}
}

void ASTPlayerController::ToggleDebuggerDisplay()
{
	if (DisplayLabel)
	{
		DisplayLabel->DisplayLabels(bIsDebuggerDisplayed);
		bIsDebuggerDisplayed = !bIsDebuggerDisplayed;
	}

	PlayerCharacter->ToggleDebuggerDisplay();
}

void ASTPlayerController::HandleDisplayLevelMenuCompleted()
{
	bIsDisplayLevelMenuCompleted = true;
}

void ASTPlayerController::HandleHideLevelMenuCompleted()
{
	bIsLevelMenuDisplayed = false;
	bIsHideLevelMenuCompleted = true;
}

void ASTPlayerController::HandleExitMenuFinished()
{
	switch (CurrentSelectedButtonType)
	{
	case EMainMenuButtonTypes::EMMBT_LevelContinue:
		IncrementAndSaveShowdownCount();
		LevelLoader->LoadRandomLevel(this);
		break;
	case EMainMenuButtonTypes::EMMBT_LevelRestart:
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
		break;
	case EMainMenuButtonTypes::EMMBT_LevelExit:
		UGameplayStatics::OpenLevel(this, FName(MAIN_MENU_MAP));
		break;
	default:
		break;
	}
}

void ASTPlayerController::HandleLevelIntroCompleted()
{
	if (SFUWLevelMenuClass)
	{
		LevelMenu = CreateWidget<USFUWLevelMenu>(GetWorld(), SFUWLevelMenuClass);
		LevelMenu->AddToViewport();
		LevelMenu->OnButtonSelected.AddDynamic(this, &ASTPlayerController::HandleButtonSelected);
		LevelMenu->OnDisplayLevelMenuCompleted.AddDynamic(this, &ASTPlayerController::HandleDisplayLevelMenuCompleted);
		LevelMenu->OnHideLevelMenuCompleted.AddDynamic(this, &ASTPlayerController::HandleHideLevelMenuCompleted);
		LevelMenu->OnExitMenuAnimFinished.AddDynamic(this, &ASTPlayerController::HandleExitMenuFinished);
		CurrentSelectedButtonType = EMainMenuButtonTypes::EMMBT_LevelResume;
	}

	bLevelIntroCompleted = true;

	
	int RandomIndex = FMath::RandRange(0, LevelMusicSounds.Num() - 1);
	CurrentVolumeLevel = MUSIC_VOLUME_MAX;
	LevelMusicAudioComponent = UGameplayStatics::SpawnSound2D(this, LevelMusicSounds[RandomIndex], MUSIC_VOLUME_MAX);
	LevelMusicAudioComponent->OnAudioFinished.AddDynamic(this, &ASTPlayerController::HandleMusicAudioFinished);
	OnLevelIntroHandled.Broadcast();
}

void ASTPlayerController::HandleAllEnemiesKilled()
{
	LevelResultType = ELevelResultType::ELRT_Completed;
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASTPlayerController::LevelResultsEvent, 3.0f, false);
}

void ASTPlayerController::HandlePlayerDied()
{
	LevelResultType = ELevelResultType::ELRT_PlayerDied;
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASTPlayerController::LevelResultsEvent, 3.0f, false);
}

void ASTPlayerController::HandleMusicAudioFinished()
{
	int RandomIndex = FMath::RandRange(0, LevelMusicSounds.Num() - 1);
	LevelMusicAudioComponent = UGameplayStatics::SpawnSound2D(this, LevelMusicSounds[RandomIndex], CurrentVolumeLevel);
}

void ASTPlayerController::IncrementAndSaveShowdownCount()
{
	if (!SaveGameData) return;
	
	FString SaveSlotName = SaveGameData->SaveSlotName;
	uint32 UserIndex = SaveGameData->UserIndex;
	SaveGameData->ShowdownCounter += 1;
	UGameplayStatics::SaveGameToSlot(SaveGameData, SaveSlotName, UserIndex);
}

void ASTPlayerController::LevelResultsEvent()
{
	LevelMenu->OnButtonSelected.RemoveDynamic(this, &ASTPlayerController::HandleButtonSelected);
	LevelMenu->OnDisplayLevelMenuCompleted.RemoveDynamic(this, &ASTPlayerController::HandleDisplayLevelMenuCompleted);
	LevelMenu->OnHideLevelMenuCompleted.RemoveDynamic(this, &ASTPlayerController::HandleHideLevelMenuCompleted);
	LevelMenu->OnExitMenuAnimFinished.RemoveDynamic(this, &ASTPlayerController::HandleExitMenuFinished);

	if (LevelResultType == ELevelResultType::ELRT_Completed)
	{
		PlayerCharacter->SwitchLevelCompleteCamera();
		PlayerCharacter->SetPlayerToEmoteState();
	}

	if (SFUWLevelResultsClass)
	{
		LevelResults = CreateWidget<USFUWLevelResults>(GetWorld(), SFUWLevelResultsClass);
		LevelResults->AddToViewport();
		LevelResults->SetupButtonsByLevelResult(LevelResultType);
		LevelResults->OnButtonSelected.AddDynamic(this, &ASTPlayerController::HandleButtonSelected);
		LevelResults->OnExitMenuAnimFinished.AddDynamic(this, &ASTPlayerController::HandleExitMenuFinished);
		CurrentSelectedButtonType = EMainMenuButtonTypes::EMMBT_LevelContinue;

		if (LevelResultType == ELevelResultType::ELRT_Completed)
		{
			PlayerCharacter->AddKatanaCover();
		}
	}
}
