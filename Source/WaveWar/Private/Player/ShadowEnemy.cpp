// Uroš Pörš All Rights Reserved


#include "Player/ShadowEnemy.h"
#include "GAS/ShadowAbilitySystemComponent.h"
#include "GAS/ShadowAttributeSet.h"
#include "UI/WW_UserWidget.h"
#include "../WaveWar.h"
#include "Game/ShadowGameMode.h"
#include "GAS/WW_GameplayTags.h"
#include "AI/WW_AIController.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"






AShadowEnemy::AShadowEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AbilitySystemComponent = CreateDefaultSubobject<UShadowAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UShadowAttributeSet>("AttributeSet");

	EnemyHealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHealthBar"));
	EnemyHealthBar->SetupAttachment(GetRootComponent());

	/** Set Enemy walk speed */
	EnemyWalkSpeed = 250.0f;

	/** Set Enemy life span */
	LifeSpan = 5.0f;
}

void AShadowEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority())
		return;

	/** Initialize BehaviorTree and runs it */
	WWAIController = Cast<AWW_AIController>(NewController);
	WWAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	WWAIController->RunBehaviorTree(BehaviorTree);
	/** Set HitReacting (in BT_EnemyBehaviorTree) as false */
	WWAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsHitReacting"), false);
	WWAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsRangeAttack"), CharacterClass != ECharacterClass::Fighter);

}

void AShadowEnemy::BeginPlay()
{
	Super::BeginPlay();

	/** Set Enemy walk speed */
	GetCharacterMovement()->MaxWalkSpeed = EnemyWalkSpeed;

	InitAbilityActorInfo();

	/** After initialize abilities, GiveStarupAbilities() function from AShadowGameMode can call */
	AShadowGameMode::GiveStarupAbilities(this, AbilitySystemComponent, CharacterClass);

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
		/** Binding callback for HitReact with adding UObject. When Enemy recive HitReact tag HitReactTagChanged() function will be call. */
		AbilitySystemComponent->RegisterGameplayTagEvent(FWWGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, &AShadowEnemy::HitReactTagChanged
		);

		/** Broadcast initial value. */
		OnHealthChanged.Broadcast(ShadowAS->GetHealth());
		OnMaxHealthChanged.Broadcast(ShadowAS->GetMaxHealth());
	}
}

int32 AShadowEnemy::GetPlayerLevel_Implementation()
{
	/** Return Enemy level */
	return Level;
}

void AShadowEnemy::Die()
{
	SetLifeSpan(LifeSpan);

	EnemyHealthBar->SetHiddenInGame(true);

	if (WWAIController)
	{
		WWAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDead"), true);
	}

	Super::Die();
}

void AShadowEnemy::SetTurnToActor_Implementation(AActor* InTurnToActor)
{
	TurnToActor = InTurnToActor;
}

AActor* AShadowEnemy::GetTurnToActor_Implementation()
{
	return TurnToActor;
}

void AShadowEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.0f : EnemyWalkSpeed;

	/** Set HitReacting whenever bHitReacting is changed. */
	if (WWAIController && WWAIController->GetBlackboardComponent())
	{
		WWAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsHitReacting"), bHitReacting);
	}
}

void AShadowEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UShadowAbilitySystemComponent>(AbilitySystemComponent)->SetAbilityActorInfo(); // Call SetAbilityActorInfo() from ShadowAbilitySystemComponent() class

	InitializeDefaultAttributes();
}

void AShadowEnemy::InitializeDefaultAttributes()
{
	/** Call InitializeDefaultAttributes() from ShadowGameMode() class. */
	AShadowGameMode::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}
