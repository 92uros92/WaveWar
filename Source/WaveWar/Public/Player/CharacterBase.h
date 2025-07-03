// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterBase.generated.h"




UCLASS(ABSTRACT)
class WAVEWAR_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:

	ACharacterBase();

protected:

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	////****	FUNCTIONS	****////

	virtual void BeginPlay() override;

};
