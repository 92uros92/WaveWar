// Uroš Pörš All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GAS/WW_GameplayAbility.h"
#include "ProjectileActivate_GA.generated.h"




class AGunProjectile;
class UGameplayEffect;


UCLASS()
class WAVEWAR_API UProjectileActivate_GA : public UWW_GameplayAbility
{
	GENERATED_BODY()
	
public:


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AGunProjectile> GunProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	////****	FUNCTIONS	****////

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	UFUNCTION(BlueprintCallable)
	void SpawnGunShoot(const FVector& HitTarget);

};
