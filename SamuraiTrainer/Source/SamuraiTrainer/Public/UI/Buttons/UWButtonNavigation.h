// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWButtonNavigation.generated.h"

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API UUWButtonNavigation : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUWButtonNavigation* SelectTopButton();
	UUWButtonNavigation* SelectBottomButton();

public:
	FORCEINLINE void SetTopButton(UUWButtonNavigation* Value) { BNTop = Value; }
	FORCEINLINE void SetBottomButton(UUWButtonNavigation* Value) { BNBottom = Value; }

protected:
	UUWButtonNavigation* BNTop;
	UUWButtonNavigation* BNBottom;
};
