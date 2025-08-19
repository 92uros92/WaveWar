// Uroš Pörš All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ShadowAbilitySystemComponent.generated.h"



DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&)


UCLASS()
class WAVEWAR_API UShadowAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:

	FEffectAssetTags EffectAssetTags;

	void SetAbilityActorInfo();

	void GiveStarupAbilities(TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

protected:

	void ApplyEffect(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
