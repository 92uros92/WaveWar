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

};
