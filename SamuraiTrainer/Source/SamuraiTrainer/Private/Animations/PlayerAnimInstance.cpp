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
	if (PlayerCharacter)
	{
		MovementComponent = PlayerCharacter->GetCharacterMovement();
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MovementComponent)
	{
		MovementSpeed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
	}
}

void UPlayerAnimInstance::OnSwordInteractionCompleted()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->SetIsInteractingWithWeapon(false);
		WeaponState = PlayerCharacter->GetWeaponState();
	}
}

void UPlayerAnimInstance::OnAttackCompleted()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->SetMovementState(EMovementStates::EPMS_Idle);
		PlayerCharacter->SetCanPerformNextAttack(true);
	}
}
