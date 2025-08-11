// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "Projectiles/GunProjectile.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"




AGunProjectile::AGunProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CapsuleComponent"));
	BoxComponent->SetCollisionProfileName("Projectile");
	RootComponent = BoxComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(BoxComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->InitialSpeed = 1000.0f;
	MovementComponent->MaxSpeed = 1000.0f;
	MovementComponent->ProjectileGravityScale = 0.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;

}

void AGunProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AGunProjectile::OnOverlap);
}

void AGunProjectile::Destroyed()
{
	if (!bIsHit && !HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShootSound, GetActorLocation(), FRotator::ZeroRotator);
	}

	Super::Destroyed();
}

void AGunProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::PlaySoundAtLocation(this, ShootSound, GetActorLocation(), FRotator::ZeroRotator);

	if (HasAuthority())
	{
		Destroy();
	}
	else
	{
		bIsHit = true;
	}
}

