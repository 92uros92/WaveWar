// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "Player/ShadowEnemy.h"
#include "GAS/ShadowAbilitySystemComponent.h"
#include "GAS/ShadowAttributeSet.h"





AShadowEnemy::AShadowEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UShadowAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UShadowAttributeSet>("AttributeSet");
}

int32 AShadowEnemy::GetPlayerLevel()
{
	/** Return Enemy level */
	return Level;
}

//void AShadowEnemy::HighlightActor()
//{
//	bHighlighted = true;
//}
//
//void AShadowEnemy::UnHighlightActor()
//{
//	bHighlighted = false;
//}

void AShadowEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();
}

void AShadowEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UShadowAbilitySystemComponent>(AbilitySystemComponent)->SetAbilityActorInfo(); // Call SetAbilityActorInfo() from ShadowAbilitySystemComponent() class
}
