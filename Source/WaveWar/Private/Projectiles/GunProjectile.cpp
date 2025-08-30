// Uroš Pörš All Rights Reserved


#include "Projectiles/GunProjectile.h"
#include "../WaveWar.h"
#include "GAS/WW_BlueprintFunctionLibrary.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"




AGunProjectile::AGunProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionObjectType(ECC_Projectile);
	BoxComponent->SetCollisionProfileName("Projectile");
	

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(BoxComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->InitialSpeed = 2000.0f;
	MovementComponent->MaxSpeed = 2000.0f;
	MovementComponent->ProjectileGravityScale = 0.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;

	/** Set gun projectile life spam to 8 second */
	LifeSpam = 8.0f;
}

void AGunProjectile::BeginPlay()
{
	Super::BeginPlay();

	/** Set projectile life span */
	SetLifeSpan(LifeSpam);
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AGunProjectile::OnOverlap);

	UGameplayStatics::PlaySoundAtLocation(this, ShootSound, GetActorLocation(), FRotator::ZeroRotator);

	if (BulletTrail)
	{
		TrailComp = UGameplayStatics::SpawnEmitterAttached(BulletTrail, BoxComponent, FName(), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition);
	}
}

void AGunProjectile::Destroyed()
{
	//if (!bIsHit && !HasAuthority())
	//{
		
	//}

	Super::Destroyed();
}

void AGunProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Destroy projectile!!"));

	if (!DamageEffectSpecHandle.Data.IsValid() || DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor)
	{
		return;
	}
	if (!UWW_BlueprintFunctionLibrary::IsNotSameTeam(DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser(), OtherActor))
	{
		return;
	}
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* DamageTarget = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			/** Apply damage effect to self. */
			DamageTarget->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}

		Destroy();
	}
	else
	{
		bIsHit = true;
	}
}

