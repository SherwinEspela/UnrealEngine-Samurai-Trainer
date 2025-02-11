// Copyright 2024 Sherwin Espela. All rights reserved.


#include "UI/Buttons/UWButtonMainMenu.h"
#include "Components/TextBlock.h"

void UUWButtonMainMenu::PlaySelect()
{
	OnButtonSelectStarted.Broadcast();
	OnPlaySelectAnimation();
}

void UUWButtonMainMenu::PlayUnselect()
{
	OnPlayUnselectAnimation();
}

void UUWButtonMainMenu::SetTitle(FString Title)
{
	TextTitle->SetText(FText::FromString(Title));
}

void UUWButtonMainMenu::SetButtonType(EMainMenuButtonTypes ButtonType)
{
	MainMenuButtonType = ButtonType;
}

void UUWButtonMainMenu::HandleSelectAnimFinished()
{
	OnButtonSelectAnimFinished.Broadcast();
}
