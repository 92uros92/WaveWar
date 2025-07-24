// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

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
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Damage, Category = "Primary Attributes")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UShadowAttributeSet, Damage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AddMaxHP, Category = "Primary Attributes")
	FGameplayAttributeData AddMaxHP;
	ATTRIBUTE_ACCESSORS(UShadowAttributeSet, AddMaxHP);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeed, Category = "Primary Attributes")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UShadowAttributeSet, MovementSpeed);
	/*
	*	END Primary Attributes
	*/

	/*
	*	Life Attributes
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Life Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UShadowAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Life Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UShadowAttributeSet, MaxHealth);
	/*
	*	END Life Attributes
	*/


	////****	FUNCTIONS	****////

	UShadowAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	/** Life Attributes "OnRep" function */
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	/** END Life Attributes "OnRep" function */

	/** Primary Attributes "OnRep" function */
	UFUNCTION()
	void OnRep_Damage(const FGameplayAttributeData& OldDamage) const;

	UFUNCTION()
	void OnRep_AddMaxHP(const FGameplayAttributeData& OldAddMaxHP) const;
	
	UFUNCTION()
	void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed) const;
	/** END Primary Attributes "OnRep" function */

};
