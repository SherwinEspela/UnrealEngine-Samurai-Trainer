// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SFUWMenuBase.h"
#include "UI/SFUWExitableMenu.h"
#include "Blueprint/UserWidget.h"
#include "SFUWLevelResults.generated.h"

class UUWButtonMainMenu;
class UUWButtonNavigation;

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API USFUWLevelResults : public USFUWExitableMenu
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

protected:
	// Buttons
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUWButtonMainMenu* BMMContinue;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUWButtonMainMenu* BMMExit;
	
};
