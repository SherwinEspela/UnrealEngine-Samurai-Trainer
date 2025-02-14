// Copyright 2024 Sherwin Espela. All rights reserved.


#include "UI/SFUWLevelIntro.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
//#include "Sound/SoundBase.h"

void USFUWLevelIntro::NativeConstruct()
{
	Super::NativeConstruct();
}

void USFUWLevelIntro::HandleLevelIntroAnimationCompleted()
{
	OnLevelIntroCompleted.Broadcast();
}

void USFUWLevelIntro::SetShowdownCount(int Value)
{
	TextShowdown->SetText(FText::FromString(FString::Printf(TEXT("Showdown %i"), Value)));
	SetRandomBackground();
}

void USFUWLevelIntro::SetRandomBackground()
{
	int RandomIndex = FMath::RandRange(0, BackgroundTextures.Num() - 1);
	ImageBG->SetBrushFromTexture(BackgroundTextures[RandomIndex], true);
}
