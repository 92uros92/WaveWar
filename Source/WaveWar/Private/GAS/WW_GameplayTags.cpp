// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "GAS/WW_GameplayTags.h"
#include "GameplayTagsManager.h"



FWWGameplayTags FWWGameplayTags::GameplayTags;


void FWWGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Attribute_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduce damage taken."));
}
