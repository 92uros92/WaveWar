// Uroš Pörš All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ShadowAttributeSet.generated.h"


/** This automatically generate getter and setter functions for Attributes (from AttributeSet.h) */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS()
class WAVEWAR_API UShadowAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	/*
	*	Primary Attributes
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Endurance, Category = "Primary Attributes")
	FGameplayAttributeData Endurance;
	ATTRIBUTE_ACCESSORS(UShadowAttributeSet, Endurance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Damage, Category = "Primary Attributes")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UShadowAttributeSet, Damage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AddMaxHP, Category = "Primary Attributes")
	FGameplayAttributeData AddMaxHP;
	ATTRIBUTE_ACCESSORS(UShadowAttributeSet, AddMaxHP);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AddMovementSpeed, Category = "Primary Attributes")
	FGameplayAttributeData AddMovementSpeed;
	ATTRIBUTE_ACCESSORS(UShadowAttributeSet, AddMovementSpeed);
	/*
	*	END Primary Attributes
	*/

	/*
	*	Secondary Attributes
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UShadowAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHit, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHit;
	ATTRIBUTE_ACCESSORS(UShadowAttributeSet, CriticalHit);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UShadowAttributeSet, BlockChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UShadowAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeed, Category = "Secondary Attributes")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UShadowAttributeSet, MovementSpeed);
	/*
	*	END Secondary Attributes
	*/

	/*
	*	Life Attributes
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Life Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UShadowAttributeSet, Health);
	/*
	*	END Life Attributes
	*/

	/*
	*	Damage Attributes (Meta Attributes) --> NOT replicated
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Damage Attributes")
	FGameplayAttributeData CalculateDamage;
	ATTRIBUTE_ACCESSORS(UShadowAttributeSet, CalculateDamage);
	/*
	*	END Damage Attributes (Meta Attributes)
	*/

	////****	FUNCTIONS	****////

	UShadowAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	/** Life Attributes "OnRep" function */
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	/** END Life Attributes "OnRep" function */

	/** Primary Attributes "OnRep" function */
	UFUNCTION()
	void OnRep_Endurance(const FGameplayAttributeData& OldEndurance) const;

	UFUNCTION()
	void OnRep_Damage(const FGameplayAttributeData& OldDamage) const;

	UFUNCTION()
	void OnRep_AddMaxHP(const FGameplayAttributeData& OldAddMaxHP) const;
	
	UFUNCTION()
	void OnRep_AddMovementSpeed(const FGameplayAttributeData& OldAddMovementSpeed) const;
	/** END Primary Attributes "OnRep" function */

	/** Secondary Attributes "OnRep" function */
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	UFUNCTION()
	void OnRep_CriticalHit(const FGameplayAttributeData& OldCriticalHit) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed) const;
	/** END Secondary Attributes "OnRep" function */
};
