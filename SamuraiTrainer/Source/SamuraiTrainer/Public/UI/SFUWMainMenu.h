// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomEnums.h"
#include "UI/SFUWMenuBase.h"
#include "Blueprint/UserWidget.h"
#include "SFUWMainMenu.generated.h"

class UUWButtonMainMenu;
class UUWButtonNavigation;
class UTextBlock;
class UHorizontalBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLogoIntroAnimFinishedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMainMenuEntryAnimFinishedSignature);

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API USFUWMainMenu : public USFUWMenuBase
{
	GENERATED_BODY()
	
public:
	void PlayLogoExit();
	void PlayEnterMainMenu();
	/*void SelectTopButton();
	void SelectBottomButton();*/

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayLogoExit();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayEnterMainMenu();

public:
	FLogoIntroAnimFinishedSignature OnLogoIntroAnimFinished;
	FMainMenuEntryAnimFinishedSignature OnMainMenuEntryAnimFinished;

protected:
	virtual void NativeConstruct() override;

protected:
	// Anim Events
	UFUNCTION(BlueprintCallable)
	void HandleLogoIntroAnimFinished();

	UFUNCTION(BlueprintCallable)
	void HandleMainMenuEntryAnimFinished();

	UFUNCTION()
	void HandleButtonSelectStarted();

	UFUNCTION()
	void HandleButtonSelectAnimFinished();

protected:
	// Buttons
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUWButtonMainMenu* BMMPlay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUWButtonMainMenu* BMMModes;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUWButtonMainMenu* BMMSettings;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUWButtonMainMenu* BMMDevBio;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUWButtonMainMenu* BMMTutorial;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* HBConfirmGroup;

private:
	void SetupButtonNavigationMapping();
	void SubscribeToButtonEvents();

};
