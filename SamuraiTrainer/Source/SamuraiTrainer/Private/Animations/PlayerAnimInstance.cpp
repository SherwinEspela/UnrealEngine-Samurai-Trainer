// // Copyright 2024 Sherwin Espela. All rights reserved.


#include "Animations/PlayerAnimInstance.h"
#include "Character/STPlayerCharacter.h"
#include "Gameframework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "CustomEnums.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	PlayerCharacter = Cast<ASTPlayerCharacter>(TryGetPawnOwner());
}

void UPlayerAnimInstance::OnSwordInteractionCompleted()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->SetIsInteractingWithWeapon(false);
		WeaponState = PlayerCharacter->GetWeaponState();
	}
}
