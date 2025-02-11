// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SFUWMenuBase.h"
#include "SFUWExitableMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExitMenuAnimFinishedSignature);

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API USFUWExitableMenu : public USFUWMenuBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnExitMenu();

	UFUNCTION(BlueprintCallable)
	void HandleExitMenuAnimFinished();

	FExitMenuAnimFinishedSignature OnExitMenuAnimFinished;

};
