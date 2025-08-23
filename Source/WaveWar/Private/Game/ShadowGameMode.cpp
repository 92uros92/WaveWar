// Uroš Pörš All Rights Reserved


#include "Game/ShadowGameMode.h"
#include "GAS/ShadowAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

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

void AShadowGameMode::GiveStarupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	AShadowGameMode* ShadowGM = Cast<AShadowGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (ShadowGM == nullptr)
		return;

	/** Get data from UCharacterClassData() class. */
	UCharacterClassData* CharacterClassData = ShadowGM->CharacterClassData;

	if (CharacterClassData == nullptr)
		return;

	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassData->Abilities)
	{
		/** Loop through Abilities array from UCharacterClassData and grants an ability. */
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}

	const FCharacterClassInfo& ClassInfo = CharacterClassData->GetCharacterClassInfo(CharacterClass);

	for (TSubclassOf<UGameplayAbility> AbilityClass : ClassInfo.StartAbilities)
	{
		ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor());

		if (CombatInterface)
		{
			/** Loop through Abilities array from UCharacterClassData and grants an strtup ability. */
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetPlayerLevel());
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

void AShadowGameMode::GetLivePlayersInRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorToIgnore, float Radius, const FVector& SphereLocation)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorToIgnore);

	// query scene to see what we hit
	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereLocation, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			bool CombatInterface = Overlap.GetActor()->Implements<UCombatInterface>();
			if (CombatInterface)
			{
				bool IsActorAlive = !ICombatInterface::Execute_IsPlayerDead(Overlap.GetActor());
				if (IsActorAlive)
				{
					OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
				}
			}
		}
	}
}

