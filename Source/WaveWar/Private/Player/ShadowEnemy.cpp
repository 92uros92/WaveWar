// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "Player/ShadowEnemy.h"
#include "GAS/ShadowAbilitySystemComponent.h"
#include "GAS/ShadowAttributeSet.h"
#include "UI/WW_UserWidget.h"
#include "../WaveWar.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"





AShadowEnemy::AShadowEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	AbilitySystemComponent = CreateDefaultSubobject<UShadowAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UShadowAttributeSet>("AttributeSet");

	EnemyHealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHealthBar"));
	EnemyHealthBar->SetupAttachment(GetRootComponent());
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

	/** Set EnemyHealtBar widget */
	UWW_UserWidget* WWUserWidget = Cast<UWW_UserWidget>(EnemyHealthBar->GetUserWidgetObject());
	if (WWUserWidget)
	{
		WWUserWidget->SetWidgetController(this);
	}

	UShadowAttributeSet* ShadowAS = Cast<UShadowAttributeSet>(AttributeSet);
	if (ShadowAS)
	{
		/** Binding callback for Health and MaxHealth with lambda. Broadcast to attribute changes. */
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ShadowAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ShadowAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		/** Broadcast initial value. */
		OnHealthChanged.Broadcast(ShadowAS->GetHealth());
		OnMaxHealthChanged.Broadcast(ShadowAS->GetMaxHealth());
	}
}

void AShadowEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UShadowAbilitySystemComponent>(AbilitySystemComponent)->SetAbilityActorInfo(); // Call SetAbilityActorInfo() from ShadowAbilitySystemComponent() class

	InitializeDefaultAttributes();
}
