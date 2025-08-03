// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "Projectiles/ProjectileActivate_GA.h"

void UProjectileActivate_GA::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("ProjectileActivate_GA"));
}
