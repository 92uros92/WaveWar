// Uroš Pörš All Rights Reserved


#include "GAS/ShadowAttributeSet.h"
#include "GAS/WW_GameplayTags.h"
#include "GAS/ShadowAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
#include "Player/ShadowPlayerController.h"
#include "GAS/WW_BlueprintFunctionLibrary.h"
#include "Interaction/ShadowInterface.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"





UShadowAttributeSet::UShadowAttributeSet()
{
	const FWWGameplayTags& GameplayTags = FWWGameplayTags::Get();

	FAttributeDelegate DamageDelegate;
	DamageDelegate.BindStatic(GetDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Primary_Damage, DamageDelegate);

	FAttributeDelegate EnduranceDelegate;
	EnduranceDelegate.BindStatic(GetEnduranceAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Primary_Endurance, EnduranceDelegate);

	FAttributeDelegate AddMaxHPDelegate;
	AddMaxHPDelegate.BindStatic(GetAddMaxHPAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Primary_AddMaxHP, AddMaxHPDelegate);

	FAttributeDelegate AddMovementSpeedDelegate;
	AddMovementSpeedDelegate.BindStatic(GetAddMovementSpeedAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Primary_AddMovementSpeed, AddMovementSpeedDelegate);
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

	/** Get SourceASC (ASC who cause the effect). */
	const FGameplayEffectContextHandle EffectContextHandle = Data.EffectSpec.GetContext();
	const UAbilitySystemComponent* SourceASC = EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (IsValid(SourceASC) && SourceASC->AbilityActorInfo.IsValid() && SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		/** Get SourceActor (the one who cause the effect). */
		AActor* SourceActor = SourceASC->AbilityActorInfo->AvatarActor.Get();

		if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
		{
			/** Get EnemyActor and EnemyCharacter (target of the effect) and EnemyASC (ASC who is target of the effect). */
			AActor* EnemyActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
			ACharacter* EnemyCharacter = Cast<ACharacter>(EnemyActor);
			UAbilitySystemComponent* EnemyASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EnemyActor);

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
						SendXPEvent(Data);
					}
					else
					{
						if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
						{
							/** Try activate ability (HitReact) if is enemy with that tag and is NOT dead. */
							FGameplayTagContainer TagContainer;
							TagContainer.AddTag(FWWGameplayTags::Get().Effects_HitReact);
							EnemyASC->TryActivateAbilitiesByTag(TagContainer);
						}
					}

					/** Get SourceCharacter (the one who cause the effect). */
					const AController* SourceController = SourceASC->AbilityActorInfo->PlayerController.Get();
					if (SourceController)
					{
						ACharacter* SourceCharacter = Cast<ACharacter>(SourceController->GetPawn());
						if (SourceCharacter)
						{
							if (SourceCharacter != EnemyCharacter)
							{
								AShadowPlayerController* ShadowPC = Cast<AShadowPlayerController>(UGameplayStatics::GetPlayerController(SourceActor, 0));
								if (ShadowPC)
								{
									/** If SourceCharacter is not equal EnemyCharacter then call ShowFloatDamageNumber() function from AShadowPlayerController. */
									ShadowPC->ShowFloatDamageNumber(LocalCalculateDamage, EnemyCharacter);
								}
							}
						}
					}
				}
			}
			if (Data.EvaluatedData.Attribute == GetCalculateXPAttribute())
			{
				const float LocalCalculateXP = GetCalculateXP();
				SetCalculateXP(0.0f);
				//UE_LOG(LogTemp, Warning, TEXT("XP: %f"), LocalCalculateXP);

				const AController* SourceController = SourceASC->AbilityActorInfo->PlayerController.Get();
				if (SourceController)
				{
					ACharacter* SourceCharacter = Cast<ACharacter>(SourceController->GetPawn());
					if (SourceCharacter->Implements<UShadowInterface>() && SourceCharacter->Implements<UCombatInterface>())
					{
						const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(SourceCharacter);
						const int32 CurrentXP = IShadowInterface::Execute_GetXP(SourceCharacter);

						const int32 NewLevel = IShadowInterface::Execute_FindLevelForXP(SourceCharacter, CurrentXP + LocalCalculateXP);
						const int32 NumberOfLevelUps = NewLevel - CurrentLevel;
						if (NumberOfLevelUps > 0)
						{
							const int32 AttributePointsAdded = IShadowInterface::Execute_GetAttributePoints(SourceCharacter, CurrentLevel);

							IShadowInterface::Execute_AddToPlayerLevel(SourceCharacter, NumberOfLevelUps);

							IShadowInterface::Execute_LevelUp(SourceCharacter);

							IShadowInterface::Execute_AddToAttributePoints(SourceCharacter, AttributePointsAdded);
						}

						/** Adding XP */
						IShadowInterface::Execute_AddToXP(SourceCharacter, LocalCalculateXP);
					}
				}
			}
		}
	}

}

void UShadowAttributeSet::SendXPEvent(const FGameplayEffectModCallbackData& Data)
{
	AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
	ACharacter* TargetCharacter = Cast<ACharacter>(TargetActor);

	const FGameplayEffectContextHandle EffectContextHandle = Data.EffectSpec.GetContext();
	const UAbilitySystemComponent* SourceASC = EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	const AController* SourceController = SourceASC->AbilityActorInfo->PlayerController.Get();
	if (SourceController)
	{
		ACharacter* SourceCharacter = Cast<ACharacter>(SourceController->GetPawn());

		if (TargetCharacter->Implements<UCombatInterface>() && SourceCharacter)
		{
			int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(TargetCharacter);
			ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(TargetCharacter);
			int32 XPReward = UWW_BlueprintFunctionLibrary::GetXPReward(TargetCharacter, TargetClass, TargetLevel);

			const FWWGameplayTags& GameplayTags = FWWGameplayTags::Get();
			FGameplayEventData Payload;
			Payload.EventTag = GameplayTags.Attribute_Meta_XP;
			Payload.EventMagnitude = XPReward;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(SourceCharacter, GameplayTags.Attribute_Meta_XP, Payload);
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

