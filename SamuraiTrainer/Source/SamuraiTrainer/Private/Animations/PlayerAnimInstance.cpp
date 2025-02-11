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

void UPlayerAnimInstance::SetRandomEmoteIdleType()
{
	int RandomIndex = FMath::RandRange(0, 4);
	switch (RandomIndex)
	{
	case 0:
		EmoteIdleType = EPlayerEmoteIdleTypes::EPEIT_LookingAround;
		break;
	case 1:
		EmoteIdleType = EPlayerEmoteIdleTypes::EPEIT_Impatient;
		break;
	case 2:
		EmoteIdleType = EPlayerEmoteIdleTypes::EPEIT_LoosenUp1;
		break;
	case 3:
		EmoteIdleType = EPlayerEmoteIdleTypes::EPEIT_LoosenUp2;
		break;
	case 4:
		EmoteIdleType = EPlayerEmoteIdleTypes::EPEIT_LoosenUp3;
		break;
	default:
		break;
	}
}
