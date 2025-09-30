// Uroš Pörš All Rights Reserved


#include "GAS/ShadowAbilitySystemComponent.h"
#include "GAS/WW_GameplayTags.h"
#include "GAS/WW_GameplayAbility.h"
#include "Interaction/ShadowInterface.h"

#include "AbilitySystemBlueprintLibrary.h"





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

void UShadowAbilitySystemComponent::GiveStarupPassiveAbilities(TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	/** Loop through GameplayAbility to grant an Ability */
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

		GiveAbilityAndActivateOnce(AbilitySpec);
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

void UShadowAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UShadowInterface>())
	{
		if (IShadowInterface::Execute_GetUpdateAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

void UShadowAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.0f;

	/** Send the event to increase attribute with available attribute points */
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

	/** Decrease available attribute points by 1 */
	if (GetAvatarActor()->Implements<UShadowInterface>())
	{
		IShadowInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
	}
}

void UShadowAbilitySystemComponent::ApplyEffect(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer GameplayTagContainer;
	EffectSpec.GetAllAssetTags(GameplayTagContainer);

	EffectAssetTags.Broadcast(GameplayTagContainer);

	
}
