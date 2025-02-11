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
#include "Components/AudioComponent.h"

#define MAIN_MENU_MAP FName("MainMenuMap")
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

	if (SFUWLevelIntroClass)
	{
		LevelIntro = CreateWidget<USFUWLevelIntro>(GetWorld(), SFUWLevelIntroClass);
		LevelIntro->AddToViewport();
		LevelIntro->OnLevelIntroCompleted.AddDynamic(this, &ASTPlayerController::HandleLevelIntroCompleted);
	}

	bIsLevelMenuDisplayed = false;
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
	if (bIsLevelCompleted) return;
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
	if (bIsLevelCompleted) return;
	if (!bLevelIntroCompleted) return;

	FVector2D LookAxisVector = Value.Get<FVector2D>();
	PlayerCharacter->AddControllerYawInput(LookAxisVector.X);
	PlayerCharacter->AddControllerPitchInput(LookAxisVector.Y);
}

void ASTPlayerController::SwordInteract()
{
	if (bIsLevelCompleted) return;
	if (!bLevelIntroCompleted) return;

	PlayerCharacter->SwordInteract();
}

void ASTPlayerController::Attack()
{
	if (bIsLevelCompleted) return;
	if (!bLevelIntroCompleted) return;

	PlayerCharacter->SwordAttack();
}

void ASTPlayerController::AttackCombo2()
{
	if (bIsLevelCompleted) return;
	if (!bLevelIntroCompleted) return;

	PlayerCharacter->SwordAttackCombo2();
}

void ASTPlayerController::Block()
{
	if (bIsLevelCompleted) return;
	if (!bLevelIntroCompleted) return;

	PlayerCharacter->Block();
}

void ASTPlayerController::ParryOrBlock()
{
	if (bIsLevelCompleted) return;
	if (!bLevelIntroCompleted) return;

	PlayerCharacter->ParryOrBlock();
}

void ASTPlayerController::Kick()
{
	if (bIsLevelCompleted) return;
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
	if (bIsLevelCompleted) return;
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
}

void ASTPlayerController::HideLevelMenu()
{
	if (!bIsLevelMenuDisplayed) return;
	if (bIsGameExiting) return;

	UGameplayStatics::SetGamePaused(GetWorld(), false);
	LevelMenu->OnHide();
	LevelMusicAudioComponent->SetVolumeMultiplier(MUSIC_VOLUME_MAX);
}

void ASTPlayerController::ExitToMainMenu()
{
	if (bIsGameExiting) return;
	bIsGameExiting = true;

	if (bIsLevelCompleted)
	{
		LevelResults->OnExitToMainMenu();
		return;
	}

	if (!bIsLevelMenuDisplayed) return;
	LevelMenu->OnExitToMainMenu();
}

void ASTPlayerController::SelectTopButton()
{
	if (bIsLevelCompleted && !bIsGameExiting)
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
	if (bIsLevelCompleted && !bIsGameExiting)
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
	if (bIsLevelCompleted && !bIsGameExiting)
	{
		switch (CurrentSelectedButtonType)
		{
		case EMainMenuButtonTypes::EMMBT_LevelContinue:
			
			break;
		case EMainMenuButtonTypes::EMMBT_LevelExit:
			ExitToMainMenu();
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
		ExitToMainMenu();
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

void ASTPlayerController::HandleExitGameFinished()
{
	UGameplayStatics::OpenLevel(this, FName(MAIN_MENU_MAP));
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
		LevelMenu->OnExitGameAnimFinished.AddDynamic(this, &ASTPlayerController::HandleExitGameFinished);
		CurrentSelectedButtonType = EMainMenuButtonTypes::EMMBT_LevelResume;
	}

	bLevelIntroCompleted = true;
	LevelMusicAudioComponent = UGameplayStatics::SpawnSound2D(this, SoundMusic, MUSIC_VOLUME_MAX);
	OnLevelIntroHandled.Broadcast();
}

void ASTPlayerController::HandleAllEnemiesKilled()
{
	bIsLevelCompleted = true;

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASTPlayerController::LevelResultsEvent, 3.0f, false);
}

void ASTPlayerController::HandlePlayerDied()
{
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASTPlayerController::LevelResultsEvent, 3.0f, false);
}

void ASTPlayerController::LevelResultsEvent()
{
	LevelMenu->OnButtonSelected.RemoveDynamic(this, &ASTPlayerController::HandleButtonSelected);
	LevelMenu->OnDisplayLevelMenuCompleted.RemoveDynamic(this, &ASTPlayerController::HandleDisplayLevelMenuCompleted);
	LevelMenu->OnHideLevelMenuCompleted.RemoveDynamic(this, &ASTPlayerController::HandleHideLevelMenuCompleted);
	LevelMenu->OnExitGameAnimFinished.RemoveDynamic(this, &ASTPlayerController::HandleExitGameFinished);

	if (SFUWLevelResultsClass)
	{
		LevelResults = CreateWidget<USFUWLevelResults>(GetWorld(), SFUWLevelResultsClass);
		LevelResults->AddToViewport();
		LevelResults->OnButtonSelected.AddDynamic(this, &ASTPlayerController::HandleButtonSelected);
		LevelResults->OnExitGameAnimFinished.AddDynamic(this, &ASTPlayerController::HandleExitGameFinished);
		CurrentSelectedButtonType = EMainMenuButtonTypes::EMMBT_LevelContinue;
	}

	if (bIsLevelCompleted)
	{
		PlayerCharacter->SwitchLevelCompleteCamera();
	}
}
