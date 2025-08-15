// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "Player/CharacterBase.h"
#include "UI/ScreenWidgetController.h"
#include "Interaction/EnemyInterface.h"
#include "Data/CharacterClassData.h"
#include "ShadowEnemy.generated.h"




class UWidgetComponent;


UCLASS()
class WAVEWAR_API AShadowEnemy : public ACharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintReadOnly)
	float EnemyWalkSpeed;

	UPROPERTY(BlueprintReadOnly)
	bool bHitReacting = false;

	////****	FUNCTIONS	****////

	AShadowEnemy();

	/** Currently don´t need that functions. */
	/*
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	*/

	/*
	*	Combat Interface
	*/
	virtual int32 GetPlayerLevel() override;
	/*
	*	END Combat Interface
	*/

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class Info")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class Info")
	ECharacterClass CharacterClass = ECharacterClass::Shooter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> EnemyHealthBar;

	////****	FUNCTIONS	****////

	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	virtual void InitializeDefaultAttributes() override;

};
