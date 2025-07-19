// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "UI/ScreenWidgetController.h"
#include "GAS/ShadowAttributeSet.h"

#include "GameplayEffectTypes.h"





void UScreenWidgetController::BroadcastInitialValues()
{
	const UShadowAttributeSet* ShadowAttributeSet = CastChecked<UShadowAttributeSet>(AttributeSet);

	/** Broadcast to Health and MaxHealth */
	OnHealthChanged.Broadcast(ShadowAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(ShadowAttributeSet->GetMaxHealth());
}

void UScreenWidgetController::InitBindingAttributes()
{
	const UShadowAttributeSet* ShadowAttributeSet = CastChecked<UShadowAttributeSet>(AttributeSet);

	/** Call bind function whenever attributes changed */
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ShadowAttributeSet->GetHealthAttribute()).AddUObject(this, &UScreenWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ShadowAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UScreenWidgetController::MaxHealthChanged);
}

void UScreenWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UScreenWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}
