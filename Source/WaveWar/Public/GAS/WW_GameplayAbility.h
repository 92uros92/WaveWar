// Uroš Pörš All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "WW_GameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class WAVEWAR_API UWW_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	/** Only for Starup InputTag (applies if ability is startup ability) */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StarupInputTag;

	/** Scalable float for Damage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FScalableFloat Damage;

};
