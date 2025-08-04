// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "Projectiles/ProjectileActivate_GA.h"
#include "Projectiles/GunProjectile.h"
#include "Interaction/CombatInterface.h"





void UProjectileActivate_GA::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer)
		return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetSocketLocation();

		/** Set location for spawning ammo */
		FTransform Transform;
		Transform.SetLocation(SocketLocation);

		/** Spawn ammo */
		AGunProjectile* Projectile = GetWorld()->SpawnActorDeferred<AGunProjectile>(
			GunProjectileClass, Transform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		Projectile->FinishSpawning(Transform);
	}

}
