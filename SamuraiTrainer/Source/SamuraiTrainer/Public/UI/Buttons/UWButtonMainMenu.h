// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Buttons/UWButtonNavigation.h"
#include "CustomEnums.h"
#include "Blueprint/UserWidget.h"
#include "UWButtonMainMenu.generated.h"

class UTextBlock;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FButtonSelectStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FButtonSelectAnimFinishedSignature);

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API UUWButtonMainMenu : public UUWButtonNavigation
{
	GENERATED_BODY()

public:
	void PlaySelect();
	void PlayUnselect();

public:
	FButtonSelectStartedSignature OnButtonSelectStarted;
	FButtonSelectAnimFinishedSignature OnButtonSelectAnimFinished;

public:
	FORCEINLINE EMainMenuButtonTypes GetButtonType() const { return MainMenuButtonType; }

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlaySelectAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayUnselectAnimation();

	UFUNCTION(BlueprintCallable)
	void HandleSelectAnimFinished();
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextTitle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* ImageTitleBG;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText TitleValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Menu Button Type")
	EMainMenuButtonTypes MainMenuButtonType;
};
