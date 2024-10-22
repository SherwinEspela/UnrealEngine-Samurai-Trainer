// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SFPlayerControllerMainMenu.generated.h"

class USFUWMainMenu;

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API ASFPlayerControllerMainMenu : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USFUWMainMenu> SFUWMainMenuClass;

	USFUWMainMenu* UWMainMenu;

};
