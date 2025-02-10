// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SFUWExitableMenu.h"
#include "Blueprint/UserWidget.h"
#include "SFUWLevelMenu.generated.h"

class UUWButtonMainMenu;
class UUWButtonNavigation;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDisplayLevelMenuCompletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHideLevelMenuCompletedSignature);

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API USFUWLevelMenu : public USFUWExitableMenu
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnDisplay();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHide();

	FDisplayLevelMenuCompletedSignature OnDisplayLevelMenuCompleted;
	FHideLevelMenuCompletedSignature OnHideLevelMenuCompleted;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void HandleDisplayLevelMenuCompleted();

	UFUNCTION(BlueprintCallable)
	void HandleHideLevelMenuCompleted();

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
