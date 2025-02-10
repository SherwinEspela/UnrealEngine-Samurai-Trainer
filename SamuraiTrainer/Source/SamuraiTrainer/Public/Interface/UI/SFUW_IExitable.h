// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SFUW_IExitable.generated.h"

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API USFUW_IExitable : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnExitToMainMenu();
};
