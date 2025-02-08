// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SFUWMenuBase.h"
#include "Blueprint/UserWidget.h"
#include "SFUWLevelMenu.generated.h"

class UUWButtonMainMenu;
class UUWButtonNavigation;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDisplayLevelMenuCompletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHideLevelMenuCompletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExitGameAnimFinishedSignature);

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API USFUWLevelMenu : public USFUWMenuBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnDisplay();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHide();

	UFUNCTION(BlueprintImplementableEvent)
	void OnExitToMainMenu();

	FDisplayLevelMenuCompletedSignature OnDisplayLevelMenuCompleted;
	FHideLevelMenuCompletedSignature OnHideLevelMenuCompleted;
	FExitGameAnimFinishedSignature OnExitGameAnimFinished;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void HandleDisplayLevelMenuCompleted();

	UFUNCTION(BlueprintCallable)
	void HandleHideLevelMenuCompleted();

	UFUNCTION(BlueprintCallable)
	void HandleExitGameAnimFinished();

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
