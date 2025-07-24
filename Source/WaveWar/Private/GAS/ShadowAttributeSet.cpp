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

	/** Life Attributes */
	/** REPNOTIFY_Always tells the OnRep function to trigger if the local value is already equal to the value being repped down from the Server */
	DOREPLIFETIME_CONDITION_NOTIFY(UShadowAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UShadowAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

	/** Primary Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UShadowAttributeSet, Damage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UShadowAttributeSet, AddMaxHP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UShadowAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
}

/** This function is called before changes actually happened (handels clamping) */
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

void UShadowAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShadowAttributeSet, Health, OldHealth);
}

void UShadowAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShadowAttributeSet, MaxHealth, OldMaxHealth);
}

void UShadowAttributeSet::OnRep_Damage(const FGameplayAttributeData& OldDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShadowAttributeSet, Damage, OldDamage);
}

void UShadowAttributeSet::OnRep_AddMaxHP(const FGameplayAttributeData& OldAddMaxHP) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShadowAttributeSet, AddMaxHP, OldAddMaxHP);
}

void UShadowAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShadowAttributeSet, MovementSpeed, OldMovementSpeed);
}
