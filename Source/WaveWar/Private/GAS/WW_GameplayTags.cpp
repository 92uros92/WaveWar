// Uroš Pörš All Rights Reserved


#include "GAS/WW_GameplayTags.h"
#include "GameplayTagsManager.h"



FWWGameplayTags FWWGameplayTags::GameplayTags;


void FWWGameplayTags::InitializeNativeGameplayTags()
{
	/*
	*	Primary Tags
	*/
	GameplayTags.Attribute_Primary_Endurance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Endurance"), FString("Reduce damage taken."));
	GameplayTags.Attribute_Primary_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Damage"), FString("Increase damage."));
	GameplayTags.Attribute_Primary_AddMaxHP = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.AddMaxHP"), FString("Increase HP."));
	GameplayTags.Attribute_Primary_AddMovementSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.AddMovementSpeed"), FString("Increase movement speed."));
	/*
	*	END Primary Tags
	*/

	/*
	*	Secondary Tags
	*/
	GameplayTags.Attribute_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduce damage taken."));
	GameplayTags.Attribute_Secondary_CriticalHit = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHit"), FString("Chance to double damage."));
	GameplayTags.Attribute_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"), FString("Chance to cut incoming damage in half."));
	GameplayTags.Attribute_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"), FString("Maximum amount of HP obtainable."));
	GameplayTags.Attribute_Secondary_MovementSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MovementSpeed"), FString("Movement speed of character."));
	/*
	*	END Secondary Tags
	*/

	/*
	*	Input Tags
	*/
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("Input Tag for left mouse button (Shoot)."));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("Input Tag for right mouse button (Aiming)."));
	/*
	*	END Input Tags
	*/

	/*
	*	Damage Tags
	*/
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString("Damage Tag for causing damage."));
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString("Pawn get Tag after to hit."));
	/*
	*	END Damage Tags
	*/

	/*
	*	Attack Tags
	*/
	GameplayTags.Attack_AbilitiesAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attack.AbilitiesAttack"), FString("Attack Tag."));
	GameplayTags.Attack_Montage_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attack.Montage.Weapon"), FString("Attack Tag for Weapon."));
	GameplayTags.Attack_Montage_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attack.Montage.RightHand"), FString("Attack Tag for attack with right hand."));
	GameplayTags.Attack_Montage_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attack.Montage.LeftHand"), FString("Attack Tag for attack with left hand."));
	GameplayTags.Attack_Montage_GunShoot = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attack.Montage.GunShoot"), FString("Attack Tag for attack with Gun."));
	/*
	*	END Attack Tags
	*/
}
