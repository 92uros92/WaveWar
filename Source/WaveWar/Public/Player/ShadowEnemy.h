// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "Player/CharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "ShadowEnemy.generated.h"




UCLASS()
class WAVEWAR_API AShadowEnemy : public ACharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

	////****	FUNCTIONS	****////

	AShadowEnemy();

	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

protected:

	virtual void BeginPlay() override;
};
