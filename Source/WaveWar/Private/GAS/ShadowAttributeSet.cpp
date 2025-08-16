// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "GAS/ShadowAttributeSet.h"
#include "GAS/WW_GameplayTags.h"
#include "GAS/ShadowAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"





UShadowAttributeSet::UShadowAttributeSet()
{
	
}

void UShadowAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/** Life Attributes */
	/** REPNOTIFY_Always tells the OnRep function to trigger if the local value is already equal to the value being repped down from the Server */
	DOREPLIFETIME_CONDITION_NOTIFY(UShadowAttributeSet, Health, COND_None, REPNOTIFY_Always);

	/** Primary Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UShadowAttributeSet, Endurance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UShadowAttributeSet, Damage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UShadowAttributeSet, AddMovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UShadowAttributeSet, AddMaxHP, COND_None, REPNOTIFY_Always);

	/** Secondary Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UShadowAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UShadowAttributeSet, CriticalHit, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UShadowAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UShadowAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UShadowAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
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

	//FEffectProperties Properties;
	//SetEffectProperties()

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetCalculateDamageAttribute())
	{
		/** Calculate damage */
		const float LocalCalculateDamage = GetCalculateDamage();
		SetCalculateDamage(0.0f);
		if (LocalCalculateDamage > 0.0f)
		{
			/** If enemy is hit set new health for enemy */
			const float NewHealth = GetHealth() - LocalCalculateDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

			const bool bIsFatal = NewHealth <= 0.0f;

			if (bIsFatal)
			{
				/** If enemy is dead --> Call Die() function from ICombatInterface */
				AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
				ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetActor);
				if (CombatInterface)
				{
					CombatInterface->Die();
				}
			}
			else
			{
				/** Try activate ability (HitReact) if is enemy with that tag and is NOT dead. */
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FWWGameplayTags::Get().Effects_HitReact);
				AActor* EnemyActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
				UAbilitySystemComponent* EnemyASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EnemyActor);
				EnemyASC->TryActivateAbilitiesByTag(TagContainer);
			}
		}
	}

}

void UShadowAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShadowAttributeSet, Health, OldHealth);
}

void UShadowAttributeSet::OnRep_Endurance(const FGameplayAttributeData& OldEndurance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShadowAttributeSet, Endurance, OldEndurance);
}

void UShadowAttributeSet::OnRep_Damage(const FGameplayAttributeData& OldDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShadowAttributeSet, Damage, OldDamage);
}

void UShadowAttributeSet::OnRep_AddMaxHP(const FGameplayAttributeData& OldAddMaxHP) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShadowAttributeSet, AddMaxHP, OldAddMaxHP);
}

void UShadowAttributeSet::OnRep_AddMovementSpeed(const FGameplayAttributeData& OldAddMovementSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShadowAttributeSet, AddMovementSpeed, OldAddMovementSpeed);
}

void UShadowAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShadowAttributeSet, Armor, OldArmor);
}

void UShadowAttributeSet::OnRep_CriticalHit(const FGameplayAttributeData& OldCriticalHit) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShadowAttributeSet, CriticalHit, OldCriticalHit);
}

void UShadowAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShadowAttributeSet, BlockChance, OldBlockChance);
}

void UShadowAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShadowAttributeSet, MaxHealth, OldMaxHealth);
}

void UShadowAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShadowAttributeSet, MovementSpeed, OldMovementSpeed);
}
