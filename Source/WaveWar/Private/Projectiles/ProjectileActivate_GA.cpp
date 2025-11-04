// Uroš Pörš All Rights Reserved


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

void UProjectileActivate_GA::SpawnGunShoot(const FVector_NetQuantize& TraceHitTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer)
		return;

	FHitResult HitResult;
	TraceUnderCrosshair(HitResult);

	const FVector SocketLocation = ICombatInterface::Execute_GetSocketLocation(GetAvatarActorFromActorInfo(), MontageTagClass);
	FVector FindTarget = TraceHitTarget - SocketLocation;

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
	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel()); // Change bact (20) to GetAbilityLevel()
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Gun Damage: %f"), ScaledDamage));
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, ScaledDamage);

	Projectile->FinishSpawning(Transform);
}

void UProjectileActivate_GA::TraceUnderCrosshair(FHitResult& HitResult)
{
	/*bool bIsLocally = GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocally)
	{
		SendGunShoot(HitResult);
	}*/

	FVector2D ViewSize;
	if (GEngine && GEngine->GameViewport)
	{
		/** Get size of the vieport */
		GEngine->GameViewport->GetViewportSize(ViewSize);
	}

	/** Make Crosshair location in the midle of the screen */
	FVector2D CrosshairLocation(ViewSize.X / 2, ViewSize.Y / 2);

	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		/** For LineTrace: Start position is on center of screen, End is 8.000 unit outworld from start position */
		FVector Start = CrosshairWorldPosition;
		FVector End = Start + (CrosshairWorldDirection * 8000.0f);

		/** It will trace strait out and it will hit first blocking hit */
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);

		/** If not get hit then HitResult = End */
		if (!HitResult.bBlockingHit)
		{
			HitResult.ImpactPoint = End;
			HitTarget = End;
		}
		else
		{
			HitTarget = HitResult.ImpactPoint;
			//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 12.0f, 12, FColor::Red);
		}
	}
}

/*void UProjectileActivate_GA::SendGunShoot(FHitResult& HitResult)
{
}
*/