// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "GAS/ShadowAbilitySystemComponent.h"





void UShadowAbilitySystemComponent::SetAbilityActorInfo()
{
	/** Bind to ApplyEffect function */
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UShadowAbilitySystemComponent::ApplyEffect);
}

void UShadowAbilitySystemComponent::ApplyEffect(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer GameplayTagContainer;
	EffectSpec.GetAllAssetTags(GameplayTagContainer);

	EffectAssetTags.Broadcast(GameplayTagContainer);

	
}
