// Copyright 2024 Sherwin Espela. All rights reserved.

#include "Character/STBaseCharacter.h"

ASTBaseCharacter::ASTBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASTBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASTBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void ASTBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}
