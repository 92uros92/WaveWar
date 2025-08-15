// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "Player/CharacterBase.h"
#include "GAS/ShadowAbilitySystemComponent.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"





ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

FVector ACharacterBase::GetSocketLocation()
{
	/** Return socket location of weapon */
	return GetMesh()->GetSocketLocation(WeaponSocketName);
}

