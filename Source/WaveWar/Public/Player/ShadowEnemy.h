// Uroš Pörš All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/CharacterBase.h"
#include "UI/ScreenWidgetController.h"
#include "Interaction/EnemyInterface.h"
#include "Data/CharacterClassData.h"
#include "ShadowEnemy.generated.h"




class UWidgetComponent;
class UBehaviorTree;
class AWW_AIController;


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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float EnemyWalkSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan;

	////****	FUNCTIONS	****////

	AShadowEnemy();

	virtual void PossessedBy(AController* NewController) override;

	/** Currently don´t need that functions. */
	/*
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	*/

	/*
	*	Combat Interface
	*/
	virtual int32 GetPlayerLevel() override;

	virtual void Die() override;
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

	UPROPERTY(EditAnywhere, category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AWW_AIController> WWAIController;

	////****	FUNCTIONS	****////

	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	virtual void InitializeDefaultAttributes() override;

};
