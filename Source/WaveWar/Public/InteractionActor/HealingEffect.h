// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealingEffect.generated.h"



class UGameplayEffect;


UCLASS()
class WAVEWAR_API AHealingEffect : public AActor
{
	GENERATED_BODY()

public:	

	AHealingEffect();

protected:

	UPROPERTY(EditAnywhere, Category = "Effect")
	TSubclassOf<UGameplayEffect> HealGameplayEffectClass;

	////****	FUNCTIONS	****////

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToActor(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

};
