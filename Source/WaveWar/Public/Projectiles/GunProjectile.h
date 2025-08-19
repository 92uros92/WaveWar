// Uroš Pörš All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "GunProjectile.generated.h"



class UBoxComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;


UCLASS()
class WAVEWAR_API AGunProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	////****	FUNCTIONS	****////

	AGunProjectile();

protected:

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	////****	FUNCTIONS	****////

	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	UPROPERTY(EditAnywhere)
	float LifeSpam;

	bool bIsHit = false;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ShootSound;
};
