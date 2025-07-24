// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "UI/ScreenWidgetController.h"
#include "GAS/ShadowAttributeSet.h"
#include "GAS/ShadowAbilitySystemComponent.h"

#include "GameplayEffectTypes.h"





void UScreenWidgetController::BroadcastInitialValues()
{
	const UShadowAttributeSet* ShadowAttributeSet = CastChecked<UShadowAttributeSet>(AttributeSet);

	/** Broadcast to Health and MaxHealth */
	OnHealthChanged.Broadcast(ShadowAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(ShadowAttributeSet->GetMaxHealth());
}

void UScreenWidgetController::InitBindingAttributes()
{
	const UShadowAttributeSet* ShadowAttributeSet = CastChecked<UShadowAttributeSet>(AttributeSet);

	/** Call lambda whenever attributes changed */
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ShadowAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ShadowAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	Cast<UShadowAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags) 
		{ 
			for (const FGameplayTag& Tag : AssetTags)
			{
				/** "A.1".MatchesTag("A") will return True, "A".MatchesTag("A.1") will return False */
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));

				if (Tag.MatchesTag(MessageTag))
				{
					/** Calling member function in lambda you need to capture this in [] */
					FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);

					MessageWidgetRow.Broadcast(*Row);
				}

				/*
				const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Msg);
				*/
			}
		}
	);
}

/** One way to do: "GetMaxHealthAttribute()).AddUObject" and then call that function. The secon one is with lambda as in InitBindingAttributes() function */
//void UScreenWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
//{
//	OnHealthChanged.Broadcast(Data.NewValue);
//}

//void UScreenWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
//{
//	OnMaxHealthChanged.Broadcast(Data.NewValue);
//}
