// Uroš Pörš All Rights Reserved


#include "GAS/ShadowAbilitySystemComponent.h"
#include "GAS/WW_GameplayTags.h"
#include "GAS/WW_GameplayAbility.h"





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
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

		if (const UWW_GameplayAbility* WWAbility = Cast<UWW_GameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(WWAbility->StarupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UShadowAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
		return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);

			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UShadowAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
		return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}

}

void UShadowAbilitySystemComponent::ApplyEffect(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer GameplayTagContainer;
	EffectSpec.GetAllAssetTags(GameplayTagContainer);

	EffectAssetTags.Broadcast(GameplayTagContainer);

	
}
