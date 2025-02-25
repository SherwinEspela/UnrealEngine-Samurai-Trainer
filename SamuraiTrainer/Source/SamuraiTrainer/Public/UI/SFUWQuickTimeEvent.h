// Copyright 2025 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerQTEResponseEnum.h"
#include "SFUWQuickTimeEvent.generated.h"

class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API USFUWQuickTimeEvent : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void DisplayWithPlayerResponseType(EPlayerQTEResponseType PlayerResponseType);
	void Hide();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnDisplayWithPlayerResponseType();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHide();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextButtonType;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageGlowInner;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageRingInner;

private:
	bool bIsDisplayed = false;

};
