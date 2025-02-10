// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SAMURAITRAINER_API IExitable
{
public:
	IExitable();
	~IExitable();

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnExitToMainMenu();
};
