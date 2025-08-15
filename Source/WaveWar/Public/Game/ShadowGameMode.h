// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/CharacterClassData.h"
#include "ShadowGameMode.generated.h"




class UCharacterClassData;
class UAbilitySystemComponent;


UCLASS()
class WAVEWAR_API AShadowGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Attributes Class|Character Class")
	TObjectPtr<UCharacterClassData> CharacterClassData;

	////****	FUNCTIONS	****////

	virtual void Tick(float DeltaSecond) override;

	/** Initialize default attributes for enemy, base on CharacterClass and Level */
	UFUNCTION(BlueprintCallable)
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	/** Initialize startup abilities for enemy */
	UFUNCTION(BlueprintCallable)
	static void GiveStarupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);

protected:


	////****	FUNCTIONS	****////


};
