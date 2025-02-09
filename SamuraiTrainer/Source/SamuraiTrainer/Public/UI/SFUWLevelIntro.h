// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SFUWLevelIntro.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelIntroCompletedSignature);

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API USFUWLevelIntro : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FLevelIntroCompletedSignature OnLevelIntroCompleted;

protected:
	UFUNCTION(BlueprintCallable)
	void HandleLevelIntroAnimationCompleted();
};
