// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SFUWLevelIntro.generated.h"

class UTextBlock;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelIntroCompletedSignature);

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API USFUWLevelIntro : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetShowdownCount(int Value);

	FLevelIntroCompletedSignature OnLevelIntroCompleted;

protected:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION(BlueprintCallable)
	void HandleLevelIntroAnimationCompleted();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextShowdown;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageBG;
};
