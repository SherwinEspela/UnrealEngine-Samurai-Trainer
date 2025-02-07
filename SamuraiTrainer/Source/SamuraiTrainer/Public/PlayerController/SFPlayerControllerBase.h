// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomEnums.h"
#include "GameFramework/PlayerController.h"
#include "SFPlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API ASFPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
	virtual void HandleButtonSelected(EMainMenuButtonTypes ButtonType);

	EMainMenuButtonTypes CurrentSelectedButtonType;
};
