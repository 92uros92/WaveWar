// Uroš Pörš All Rights Reserved


#include "Projectiles/GunProjectile.h"
#include "../WaveWar.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"




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
	MovementComponent->InitialSpeed = 1000.0f;
	MovementComponent->MaxSpeed = 1000.0f;
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
	
	UE_LOG(LogTemp, Warning, TEXT("Destroy projectile!!"));
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

