// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "InteractionActor/HealingEffect.h"
#include "GAS/ShadowAttributeSet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"




AHealingEffect::AHealingEffect()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(Mesh);

}

void AHealingEffect::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AHealingEffect::OnOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AHealingEffect::OnEndOverlap);
}

void AHealingEffect::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IAbilitySystemInterface* ASInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UShadowAttributeSet* ShadowAS = Cast<UShadowAttributeSet>(ASInterface->GetAbilitySystemComponent()->GetAttributeSet(UShadowAttributeSet::StaticClass()));
		UShadowAttributeSet* ConShadowAS = const_cast<UShadowAttributeSet*>(ShadowAS);
		ConShadowAS->SetHealth(ShadowAS->GetHealth() + 25.0f);
		Destroy();
	}
}

void AHealingEffect::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
