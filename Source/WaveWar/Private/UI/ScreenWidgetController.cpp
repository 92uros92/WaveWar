// Uroš Pörš All Rights Reserved


#include "UI/ScreenWidgetController.h"
#include "GAS/ShadowAttributeSet.h"
#include "GAS/ShadowAbilitySystemComponent.h"
#include "Player/ShadowPlayerState.h"
#include "Data/LevelUpInfo.h"

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
	AShadowPlayerState* ShadowPlayerState = CastChecked<AShadowPlayerState>(PlayerState);
	ShadowPlayerState->OnXPChangedDelegate.AddUObject(this, &UScreenWidgetController::OnXPChanged);

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

void UScreenWidgetController::OnXPChanged(int32 NewXP)
{
	AShadowPlayerState* ShadowPlayerState = CastChecked<AShadowPlayerState>(PlayerState);

	ULevelUpInfo* LevelUpInfo = ShadowPlayerState->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("LevelUpInfo Undefined. Fill out BP_ShadowPlayerState."));

	/** Get Level and MaxLevel from ULevelUpInfo */
	int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		/** Get level up requirement and previous level up requirement */
		int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		/** Get requirement XP this for level (DeltaLevelRequirement) and remind XP for this level */
		int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		/** Get the percent for this level */
		float XPPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);

		OnXPPercentChanged.Broadcast(XPPercent);
	}
}

/** One way to do: "GetMaxHealthAttribute()).AddUObject" and then call that function. The second one is with lambda as in InitBindingAttributes() function */
//void UScreenWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
//{
//	OnHealthChanged.Broadcast(Data.NewValue);
//}

//void UScreenWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
//{
//	OnMaxHealthChanged.Broadcast(Data.NewValue);
//}
