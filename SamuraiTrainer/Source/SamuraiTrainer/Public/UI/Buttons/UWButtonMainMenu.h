// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWButtonMainMenu.generated.h"

class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API UUWButtonMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextTitle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* ImageTitleBG;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText TitleValue;
};
