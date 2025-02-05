// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SFUWMenuBase.h"
#include "Blueprint/UserWidget.h"
#include "SFUWLevelMenu.generated.h"

class UUWButtonMainMenu;
class UUWButtonNavigation;
class UTextBlock;

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API USFUWLevelMenu : public USFUWMenuBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

protected:
	// Buttons
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUWButtonMainMenu* BMMResume;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUWButtonMainMenu* BMMControls;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUWButtonMainMenu* BMMSettings;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUWButtonMainMenu* BMMExit;
};
