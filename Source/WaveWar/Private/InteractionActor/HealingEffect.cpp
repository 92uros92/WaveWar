// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "InteractionActor/HealingEffect.h"
#include "GAS/ShadowAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"




AHealingEffect::AHealingEffect()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AHealingEffect::BeginPlay()
{
	Super::BeginPlay();

}

/** Call in the Blueprint */
void AHealingEffect::ApplyEffectToActor(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (TargetAbilitySystemComponent == nullptr)
		return;
	
	check(GameplayEffectClass);

	/** Create effect for the owner of this ability --> get an outgoing effect that is ready to be applied --> Applies a previously created gameplay effect spec */
	FGameplayEffectContextHandle EffectContextHandle = TargetAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContextHandle);
	TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

}

