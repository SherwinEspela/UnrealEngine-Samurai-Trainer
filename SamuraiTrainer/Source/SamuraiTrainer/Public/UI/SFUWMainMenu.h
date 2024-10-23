// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SFUWMainMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLogoIntroAnimFinishedSignature);

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API USFUWMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void PlayLogoExit();
	void PlayEnterMainMenu();

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayLogoExit();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayEnterMainMenu();

public:
	FLogoIntroAnimFinishedSignature OnLogoIntroAnimFinished;

protected:
	// Anim Events

	UFUNCTION(BlueprintCallable)
	void HandleLogoIntroAnimFinished();

};
