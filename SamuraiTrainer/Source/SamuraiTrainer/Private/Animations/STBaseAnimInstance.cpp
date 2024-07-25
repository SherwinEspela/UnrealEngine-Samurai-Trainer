// // Copyright 2024 Sherwin Espela. All rights reserved.


#include "Animations/STBaseAnimInstance.h"
#include "Gameframework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/STBaseCharacter.h"

void USTBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BaseCharacter = Cast<ASTBaseCharacter>(TryGetPawnOwner());
	if (BaseCharacter)
	{
		MovementComponent = BaseCharacter->GetCharacterMovement();
	}
}

void USTBaseAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (BaseCharacter)
	{
		FRotator AimRotation = BaseCharacter->GetBaseAimRotation();
		MovementRotation = UKismetMathLibrary::MakeRotFromX(BaseCharacter->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

		if (MovementComponent)
		{
			MovementSpeed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
		}
	}

	
}
