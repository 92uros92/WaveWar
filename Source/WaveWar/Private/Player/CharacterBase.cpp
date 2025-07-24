// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "Player/CharacterBase.h"

#include "AbilitySystemComponent.h"





ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::InitAbilityActorInfo()
{
	/** It is override in the child class */
}

void ACharacterBase::InitializePrimaryAttributes() const
{
	/** Check for AbilitySystemComponent and TSubclassOf DefaultPrimaryAttributes */
	check(IsValid(GetAbilitySystemComponent()));
	check(DefaultPrimaryAttributes);

	/** Create EffectContext for the owner of this ability system --> get an outgoing effect that is ready to be applied --> Applies a previously created gameplay effect spec */
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryAttributes, 1.0f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

