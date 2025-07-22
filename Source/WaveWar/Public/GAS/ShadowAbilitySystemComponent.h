// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

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

protected:

	void ApplyEffect(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
