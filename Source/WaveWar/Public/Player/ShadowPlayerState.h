// Uroš Pörš All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "ShadowPlayerState.generated.h"



class UAbilitySystemComponent;
class UAttributeSet;


DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /** StatValue */)


UCLASS()
class WAVEWAR_API AShadowPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:

	FOnPlayerStatChanged OnXPChangedDelegate;

	FOnPlayerStatChanged OnLevelChangedDelegate;

	////****	FUNCTIONS	****////

	AShadowPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet() const 
	{ 
		return AttributeSet; 
	}

	FORCEINLINE int32 GetPlayerLevel() const
	{
		return Level;
	}

	FORCEINLINE int32 GetXP() const
	{
		return XP;
	}

	void AddToXP(int32 InXP);
	void AddToLevel(int32 InLevel);

	void SetXP(int32 InXP);
	void SetLevel(int32 InLevel);

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	////****	FUNCTIONS	****////


private:

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level, Category = "Level")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP, Category = "Level")
	int32 XP = 1;

	////****	FUNCTIONS	****////

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_XP(int32 OldXP);
};
