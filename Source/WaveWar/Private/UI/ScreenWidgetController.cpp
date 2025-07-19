// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "UI/ScreenWidgetController.h"
#include "GAS/ShadowAttributeSet.h"





void UScreenWidgetController::BroadcastInitialValues()
{
	const UShadowAttributeSet* ShadowAttributeSet = CastChecked<UShadowAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(ShadowAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(ShadowAttributeSet->GetMaxHealth());
}
