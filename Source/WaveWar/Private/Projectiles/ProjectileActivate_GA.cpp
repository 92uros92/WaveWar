// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "Projectiles/ProjectileActivate_GA.h"
#include "Projectiles/GunProjectile.h"
#include "Interaction/CombatInterface.h"
#include "GAS/WW_GameplayTags.h"

#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

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

		/** Set damage gameplay effect on projectile. */
		UAbilitySystemComponent* DamageSource = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectSpecHandle SpecHandle = DamageSource->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), DamageSource->MakeEffectContext());
		Projectile->DamageEffectSpecHandle = SpecHandle;

		/** Get GameplayTag from WW_GameplayTags --> assign magnitude to tag (set in GE_Damage) --> get ScaledDamage from WW_GameplayAbility */
		FWWGameplayTags GameplayTags = FWWGameplayTags::Get();
		const float ScaledDamage = Damage.GetValueAtLevel(20); // Change bact (20) to GetAbilityLevel()
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Gun Damage: %f"), ScaledDamage));
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, ScaledDamage);

		Projectile->FinishSpawning(Transform);
	}
}

