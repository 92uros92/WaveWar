// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "GAS/ShadowAttributeSet.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"





UShadowAttributeSet::UShadowAttributeSet()
{
	InitHealth(70.0f);
	InitMaxHealth(100.0f);
}

void UShadowAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/** REPNOTIFY_Always tells the OnRep function to trigger if the local value is already equal to the value being repped down from the Server */
	DOREPLIFETIME_CONDITION_NOTIFY(UShadowAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UShadowAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UShadowAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShadowAttributeSet, Health, OldHealth);
}

void UShadowAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShadowAttributeSet, MaxHealth, OldMaxHealth);
}

/** This function is called before changes actually happened */
void UShadowAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
}

void UShadowAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	
}
