// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"


/** Singleton for GameplayTags */

struct FWWGameplayTags
{

public:

	/*
	*	Primary Tags
	*/
	FGameplayTag Attribute_Primary_Endurance;

	FGameplayTag Attribute_Primary_Damage;

	FGameplayTag Attribute_Primary_AddMaxHP;

	FGameplayTag Attribute_Primary_AddMovementSpeed;
	/*
	*	END Primary Tags
	*/

	/*
	*	Secondary Tags
	*/
	FGameplayTag Attribute_Secondary_Armor;

	FGameplayTag Attribute_Secondary_CriticalHit;

	FGameplayTag Attribute_Secondary_BlockChance;

	FGameplayTag Attribute_Secondary_MaxHealth;

	FGameplayTag Attribute_Secondary_MovementSpeed;
	/*
	*	END Secondary Tags
	*/

	/*
	*	Input Tags
	*/
	FGameplayTag InputTag_LMB;

	FGameplayTag InputTag_RMB;
	/*
	*	END Input Tags
	*/

	////****	FUNCTIONS	****////

	static const FWWGameplayTags Get()
	{
		return GameplayTags;
	}

	static void InitializeNativeGameplayTags();

private:

	static FWWGameplayTags GameplayTags;
};