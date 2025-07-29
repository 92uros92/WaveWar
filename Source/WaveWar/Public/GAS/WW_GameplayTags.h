// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"


/** Singleton for GameplayTags */

struct FWWGameplayTags
{

public:

	FGameplayTag Attribute_Secondary_Armor;

	////****	FUNCTIONS	****////

	static const FWWGameplayTags Get()
	{
		return GameplayTags;
	}

	static void InitializeNativeGameplayTags();

private:

	static FWWGameplayTags GameplayTags;
};