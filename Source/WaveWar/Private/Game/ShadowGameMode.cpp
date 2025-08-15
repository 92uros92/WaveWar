// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "Game/ShadowGameMode.h"
#include "GAS/ShadowAbilitySystemComponent.h"

#include "Kismet/GameplayStatics.h"





void AShadowGameMode::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);

}

void AShadowGameMode::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AShadowGameMode* ShadowGM = Cast<AShadowGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (ShadowGM == nullptr)
		return;

	/** Source object */
	AActor* AvatarActor = ASC->GetAvatarActor();

	/** Get data from UCharacterClassData() class. */
	UCharacterClassData* CharacterClassData = ShadowGM->CharacterClassData;
	FCharacterClassInfo ClassInfo = CharacterClassData->GetCharacterClassInfo(CharacterClass);
	
	/** Make effect context --> Make outgoing spec that is ready to be apply --> And apply effect to self (FOR PRIMARY ATTRIBUTES) */
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	/** Make effect context --> Make outgoing spec that is ready to be apply --> And apply effect to self (FOR SECONDARY ATTRIBUTES) */
	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassData->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	/** Make effect context --> Make outgoing spec that is ready to be apply --> And apply effect to self (FOR LIFE ATTRIBUTES) */
	FGameplayEffectContextHandle LifeAttributesContextHandle = ASC->MakeEffectContext();
	LifeAttributesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle LifeAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassData->LifeAttributes, Level, LifeAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*LifeAttributesSpecHandle.Data.Get());
}

void AShadowGameMode::GiveStarupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	AShadowGameMode* ShadowGM = Cast<AShadowGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (ShadowGM == nullptr)
		return;

	/** Get data from UCharacterClassData() class. */
	UCharacterClassData* CharacterClassData = ShadowGM->CharacterClassData;

	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassData->Abilities)
	{
		/** Loop through Abilities array from UCharacterClassData and grants an ability. */
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
}

