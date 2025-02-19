// Copyright 2025 Sherwin Espela. All rights reserved.

#include "UI/SFUWQuickTimeEvent.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "ConstantValues.h"

#define TEXT_BUTTON_X TEXT("X")
#define TEXT_BUTTON_Y TEXT("Y")
#define TEXT_BUTTON_A TEXT("A")
#define TEXT_BUTTON_B TEXT("B")

void USFUWQuickTimeEvent::DisplayWithPlayerResponseType(EPlayerQTEResponseType PlayerResponseType)
{
	FLinearColor LinearColor = ATTACK_INDICATOR_COLOR_GREEN;
	FString StringButtonType(TEXT_BUTTON_X);

	switch (PlayerResponseType)
	{
	case EPlayerQTEResponseType::EPQTER_SwordAttack1:
		StringButtonType = TEXT_BUTTON_X;
		LinearColor = ATTACK_INDICATOR_COLOR_BLUE;
		break;
	case EPlayerQTEResponseType::EPQTER_SwordAttack2:
		StringButtonType = TEXT_BUTTON_A;
		LinearColor = ATTACK_INDICATOR_COLOR_GREEN;
		break;
	case EPlayerQTEResponseType::EPQTER_Block:
		StringButtonType = TEXT_BUTTON_Y;
		LinearColor = ATTACK_INDICATOR_COLOR_YELLOW;
		break;
	case EPlayerQTEResponseType::EPQTER_Evade:
		StringButtonType = TEXT_BUTTON_B;
		LinearColor = ATTACK_INDICATOR_COLOR_RED;
		break;
	default:
		break;
	}

	TextButtonType->SetText(FText::FromString(StringButtonType));
	ImageGlowInner->SetColorAndOpacity(LinearColor);
	ImageRingInner->SetColorAndOpacity(LinearColor);

	OnDisplayWithPlayerResponseType();
}

void USFUWQuickTimeEvent::Hide()
{
	OnHide();
}
