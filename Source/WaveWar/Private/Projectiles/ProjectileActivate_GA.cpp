// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "Projectiles/ProjectileActivate_GA.h"
#include "Projectiles/GunProjectile.h"
#include "Interaction/CombatInterface.h"

#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

#include "DrawDebugHelpers.h"





void UProjectileActivate_GA::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void UProjectileActivate_GA::SpawnGunShoot(const FVector& HitTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer)
		return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetSocketLocation();
		FVector FindTarget = HitTarget - SocketLocation;

		/** Get Camera location --> get View location --> From socket to hit location */
		//FVector CameraLocation = CombatInterface->GetCameraLocation();
		//FVector ViewLocation = CameraLocation + (HitTarget * 5000.0f);
		FRotator ProjectilRotation = FindTarget.Rotation();

		/** Set location for spawning ammo */
		FTransform Transform;
		Transform.SetLocation(SocketLocation);
		Transform.SetRotation(ProjectilRotation.Quaternion());

		/** Spawn ammo */
		AGunProjectile* Projectile = GetWorld()->SpawnActorDeferred<AGunProjectile>(
			GunProjectileClass, Transform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		Projectile->FinishSpawning(Transform);
	}
}

/** TODO:
*		- Make Crosshair trace
*		- Use HitTarget
*/
