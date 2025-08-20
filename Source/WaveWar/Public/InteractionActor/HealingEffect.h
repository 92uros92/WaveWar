// Uroš Pörš All Rights Reserved

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TSubclassOf<UGameplayEffect> HealGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	bool bApplyEffectToEnemy = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	bool bDestroyEffectOnOverlap = false;

	////****	FUNCTIONS	****////

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToActor(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

};
