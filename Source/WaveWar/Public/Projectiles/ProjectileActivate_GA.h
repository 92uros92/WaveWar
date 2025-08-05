// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "GAS/WW_GameplayAbility.h"
#include "ProjectileActivate_GA.generated.h"




class AGunProjectile;


UCLASS()
class WAVEWAR_API UProjectileActivate_GA : public UWW_GameplayAbility
{
	GENERATED_BODY()
	
public:

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AGunProjectile> GunProjectileClass;

	////****	FUNCTIONS	****////

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	UFUNCTION(BlueprintCallable)
	void SpawnGunShoot();
};
