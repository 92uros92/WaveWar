// Uroš Pörš All Rights Reserved

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
	*	Meta Tags
	*/
	FGameplayTag Attribute_Meta_XP;
	/*
	*	END Meta Tags
	*/

	/*
	*	Input Tags
	*/
	FGameplayTag InputTag_LMB;

	FGameplayTag InputTag_RMB;
	/*
	*	END Input Tags
	*/

	/*
	*	Damage Tags
	*/
	FGameplayTag Damage;

	FGameplayTag Effects_HitReact;
	/*
	*	END Damage Tags
	*/

	/*
	*	Attack Tags
	*/
	FGameplayTag Attack_AbilitiesAttack;

	FGameplayTag Attack_Montage_Weapon;

	FGameplayTag Attack_Montage_RightHand;

	FGameplayTag Attack_Montage_LeftHand;

	FGameplayTag Attack_Montage_GunShoot;
	/*
	*	END Attack Tags
	*/

	/*
	*	Widget Tags --> Stack
	*/
	FGameplayTag Menu_WidgetStack_PopUp;

	FGameplayTag Menu_WidgetStack_GameMenu;

	FGameplayTag Menu_WidgetStack_GameHUD;

	FGameplayTag Menu_WidgetStack_MainMenu;
	/*
	*	END Widget Tags --> Stack
	*/

	/*
	*	MainMenu Widget Tags
	*/
	FGameplayTag Menu_Widget_PressAnyKey;

	FGameplayTag Menu_Widget_MainMenuScreen;

	FGameplayTag Menu_Widget_ConfirmScreen;

	FGameplayTag Menu_Widget_GameMenuScreen;
	/*
	*	END MainMenu Widget Tags
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