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

void ASTPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ASTPlayerCharacter>(GetPawn());

	UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	PlayerSubsystem->AddMappingContext(InputMappingContext, 0);

	TArray<AActor*> DisplayLabelActors;
	UGameplayStatics::GetAllActorsWithTag(this, FName("DisplayLabel"), DisplayLabelActors);

	if (DisplayLabelActors.Num() > 0)
	{
		DisplayLabel = Cast<ADisplayLabelActor>(DisplayLabelActors[0]);
	}

	if (SFUWLevelMenuClass)
	{
		LevelMenu = CreateWidget<USFUWLevelMenu>(GetWorld(), SFUWLevelMenuClass);
		LevelMenu->AddToViewport();
		LevelMenu->OnButtonSelected.AddDynamic(this, &ASTPlayerController::HandleButtonSelected);
		LevelMenu->OnDisplayLevelMenuCompleted.AddDynamic(this, &ASTPlayerController::HandleDisplayLevelMenuCompleted);
		LevelMenu->OnHideLevelMenuCompleted.AddDynamic(this, &ASTPlayerController::HandleHideLevelMenuCompleted);
		LevelMenu->OnExitGameAnimFinished.AddDynamic(this, &ASTPlayerController::HandleExitGameFinished);
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
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	PlayerCharacter->AddControllerYawInput(LookAxisVector.X);
	PlayerCharacter->AddControllerPitchInput(LookAxisVector.Y);
}

void ASTPlayerController::SwordInteract()
{
	PlayerCharacter->SwordInteract();
}

void ASTPlayerController::Attack()
{
	PlayerCharacter->SwordAttack();
}

void ASTPlayerController::AttackCombo2()
{
	PlayerCharacter->SwordAttackCombo2();
}

void ASTPlayerController::Block()
{
	PlayerCharacter->Block();
}

void ASTPlayerController::ParryOrBlock()
{
	PlayerCharacter->ParryOrBlock();
}

void ASTPlayerController::Kick()
{
	PlayerCharacter->Kick();
}

void ASTPlayerController::RestartLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void ASTPlayerController::DisplayLevelMenu()
{
	if (!LevelMenu) return;
	if (bIsLevelMenuDisplayed) return;
	if (!bIsHideLevelMenuCompleted) return;
	if (bIsGameExiting) return;
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	CurrentSelectedButtonType = EMainMenuButtonTypes::EMMBT_LevelResume;
	LevelMenu->OnDisplay();
	bIsLevelMenuDisplayed = true;
	bIsDisplayLevelMenuCompleted = false;
	bIsHideLevelMenuCompleted = false;
}

void ASTPlayerController::HideLevelMenu()
{
	if (!LevelMenu && !bIsLevelMenuDisplayed) return;
	if (bIsGameExiting) return;
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	LevelMenu->OnHide();
}

void ASTPlayerController::ExitToMainMenu()
{
	if (!LevelMenu && !bIsLevelMenuDisplayed) return;
	if (bIsGameExiting) return;
	LevelMenu->OnExitToMainMenu();
	bIsGameExiting = true;
}

void ASTPlayerController::SelectTopButton()
{
	if (!bIsLevelMenuDisplayed) return;
	if (!bIsDisplayLevelMenuCompleted) return;
	if (bIsGameExiting) return;
	LevelMenu->SelectTopButton();
}

void ASTPlayerController::SelectBottomButton()
{
	if (!bIsLevelMenuDisplayed) return;
	if (!bIsDisplayLevelMenuCompleted) return;
	if (bIsGameExiting) return;
	LevelMenu->SelectBottomButton();
}

void ASTPlayerController::ConfirmSelectedButton()
{
	if (!LevelMenu) return;
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
	UGameplayStatics::OpenLevel(this, FName("MainMenuMap"));
}
