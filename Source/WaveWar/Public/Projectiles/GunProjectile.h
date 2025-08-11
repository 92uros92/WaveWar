// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunProjectile.generated.h"



class UBoxComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;


UCLASS()
class WAVEWAR_API AGunProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

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

	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	bool bIsHit = false;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ShootSound;
};
