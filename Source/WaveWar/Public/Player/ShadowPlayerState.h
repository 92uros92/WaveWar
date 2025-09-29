// Uroš Pörš All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "ShadowPlayerState.generated.h"



class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;


DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /** StatValue */)


UCLASS()
class WAVEWAR_API AShadowPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:

	FOnPlayerStatChanged OnXPChangedDelegate;

	FOnPlayerStatChanged OnLevelChangedDelegate;

	FOnPlayerStatChanged OnAttributePointChangeDelegate;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

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

	FORCEINLINE int32 GetAttributePoint() const
	{
		return AttributePoint;
	}

	void AddToXP(int32 InXP);
	void AddToLevel(int32 InLevel);
	void AddToAttributePoint(int32 InAttributePoint);

	void SetXP(int32 InXP);
	void SetLevel(int32 InLevel);
	void SetAttributePoint(int32 InAttributePoint);

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

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoint, Category = "Level")
	int32 AttributePoint = 0;

	////****	FUNCTIONS	****////

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UFUNCTION()
	void OnRep_AttributePoint(int32 OldAttributePoint);
};
