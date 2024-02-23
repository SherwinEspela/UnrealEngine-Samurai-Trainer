// // Copyright 2024 Sherwin Espela. All rights reserved.


#include "PlayerController/STPlayerController.h"
#include "Character/STPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

void ASTPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ASTPlayerCharacter>(GetPawn());

	UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	PlayerSubsystem->AddMappingContext(InputMappingContext, 0);
}

void ASTPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
}
