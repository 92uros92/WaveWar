// Uroš Pörš All Rights Reserved


#include "Player/CharacterBase.h"
#include "GAS/ShadowAbilitySystemComponent.h"
#include "GAS/WW_GameplayTags.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"





ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bIsDead = false;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* ACharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void ACharacterBase::Die()
{
	/** Detach weapon from character and call MulticastDeath() for client */
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastDeath();
}

/** It will be set in server and clien */
void ACharacterBase::MulticastDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bIsDead = true;
}

void ACharacterBase::InitAbilityActorInfo()
{
	/** It is override in the child class */
}

void ACharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	/** Check for AbilitySystemComponent and TSubclassOf GameplayEffectClass */
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);

	/** Create EffectContext for the owner of this ability system --> get an outgoing effect that is ready to be applied --> Applies a previously created gameplay effect spec */
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ACharacterBase::InitializeDefaultAttributes()
{
	/** Call ApplyEffectToSelf function for both Primary and Secondary Attributes */
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.0f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.0f);
	ApplyEffectToSelf(DefaultLifeAttributes, 1.0f);
}

void ACharacterBase::AddCharacterAbilities()
{
	UShadowAbilitySystemComponent* ShadowASC = CastChecked<UShadowAbilitySystemComponent>(AbilitySystemComponent);

	if (!HasAuthority())
		return;

	/** Call function from ShadowAbilitySystemComponent class */
	ShadowASC->GiveStarupAbilities(StartupAbilities);
}

FVector ACharacterBase::GetSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	/** Return socket location based on MontageTag */
	const FWWGameplayTags& GameplayTags = FWWGameplayTags::Get();
	if (MontageTag.MatchesTagExact(GameplayTags.Attack_Montage_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Attack_Montage_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Attack_Montage_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Attack_Montage_GunShoot))
	{
		return GetMesh()->GetSocketLocation(GunSocketName);
	}

	return FVector();
}

bool ACharacterBase::IsPlayerDead_Implementation() const
{
	return bIsDead;
}

AActor* ACharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FMontageForAttack> ACharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

