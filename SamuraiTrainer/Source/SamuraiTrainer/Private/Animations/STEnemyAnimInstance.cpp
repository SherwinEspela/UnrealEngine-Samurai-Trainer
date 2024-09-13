// // Copyright 2024 Sherwin Espela. All rights reserved.


#include "Animations/STEnemyAnimInstance.h"
#include "Character/STEnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void USTEnemyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (BaseCharacter)
	{
		FRotator AimRotation = BaseCharacter->GetActorRotation();
		//MovementRotation = UKismetMathLibrary::MakeRotFromX(BaseCharacter->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw * -1;
		//UE_LOG(LogTemp, Warning, TEXT("MovementOffsetYaw ===== %f"), MovementOffsetYaw);
	}
}
