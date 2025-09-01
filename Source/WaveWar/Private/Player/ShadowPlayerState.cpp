// Uroš Pörš All Rights Reserved


#include "Player/ShadowPlayerState.h"
#include "GAS/ShadowAbilitySystemComponent.h"
#include "GAS/ShadowAttributeSet.h"

#include "Net/UnrealNetwork.h"





AShadowPlayerState::AShadowPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UShadowAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UShadowAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.0f;
}

void AShadowPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShadowPlayerState, Level);
	DOREPLIFETIME(AShadowPlayerState, XP);
}

UAbilitySystemComponent* AShadowPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AShadowPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;

	OnXPChangedDelegate.Broadcast(XP);
}

void AShadowPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;

	OnLevelChangedDelegate.Broadcast(Level);
}

void AShadowPlayerState::SetXP(int32 InXP)
{
	XP = InXP;

	OnXPChangedDelegate.Broadcast(XP);
}

void AShadowPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;

	OnLevelChangedDelegate.Broadcast(Level);
}

void AShadowPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void AShadowPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}
