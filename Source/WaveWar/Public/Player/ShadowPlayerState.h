// Uroš Pörš All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "ShadowPlayerState.generated.h"



class UAbilitySystemComponent;
class UAttributeSet;


UCLASS()
class WAVEWAR_API AShadowPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:



	////****	FUNCTIONS	****////

	AShadowPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	FORCEINLINE int32 GetPlayerLevel() const
	{
		return Level;
	}

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	////****	FUNCTIONS	****////


private:

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level, Category = "Level")
	int32 Level = 1;

	////****	FUNCTIONS	****////

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
};
