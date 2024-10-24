// Copyright 2024 Sherwin Espela. All rights reserved.


#include "PlayerController/STPlayerController.h"
#include "Character/STPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DisplayLabelActor.h"
#include "Combat/TargetLockActor.h"

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
	//EnhancedInputComponent->BindAction(InputActionKick, ETriggerEvent::Triggered, this, &ASTPlayerController::Kick);
	//EnhancedInputComponent->BindAction(InputActionCounter, ETriggerEvent::Triggered, this, &ASTPlayerController::Counter);
	EnhancedInputComponent->BindAction(InputActionRestartLevel, ETriggerEvent::Triggered, this, &ASTPlayerController::RestartLevel);
	EnhancedInputComponent->BindAction(InputActionToggleDebuggerDisplay, ETriggerEvent::Triggered, this, &ASTPlayerController::ToggleDebuggerDisplay);
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

//void ASTPlayerController::Counter()
//{
//	PlayerCharacter->Counter();
//}

void ASTPlayerController::RestartLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
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
