// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "Projectiles/GunProjectile.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"




AGunProjectile::AGunProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CapsuleComponent"));
	RootComponent = BoxComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(BoxComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->InitialSpeed = 1000.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;

}

void AGunProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGunProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

