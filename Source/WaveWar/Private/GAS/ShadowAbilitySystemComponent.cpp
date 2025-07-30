// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "GAS/ShadowAbilitySystemComponent.h"
#include "GAS/WW_GameplayTags.h"





void UShadowAbilitySystemComponent::SetAbilityActorInfo()
{
	/** Bind to ApplyEffect function */
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UShadowAbilitySystemComponent::ApplyEffect);

	/** Get the GameplayTags from WWGameplayTags Singleton */
	//const FWWGameplayTags& GameplayTags = FWWGameplayTags::Get();
}

void UShadowAbilitySystemComponent::GiveStarupAbilities(TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	/** Loop through GameplayAbility to grant an Ability */
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		const FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbility(AbilitySpec);
	}
}

void UShadowAbilitySystemComponent::ApplyEffect(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer GameplayTagContainer;
	EffectSpec.GetAllAssetTags(GameplayTagContainer);

	EffectAssetTags.Broadcast(GameplayTagContainer);

	
}
