// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassData.generated.h"



class UGameplayEffect;


/** Enum to dermine Character class (in DA_CharacterClass) */
UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Shooter,
	Archer,
	Fighter
};

/** Struct to select GameplayEffect for selected CharacterClass (in DA_CharacterClass) */
USTRUCT(BlueprintType)
struct FCharacterClassInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Attributes Class")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
};




UCLASS()
class WAVEWAR_API UCharacterClassData : public UDataAsset
{
	GENERATED_BODY()
	
public:

	/** For ECharacterClass choose FCharacterClassInfo */
	UPROPERTY(EditDefaultsOnly, Category = "Attributes Class|Character Class")
	TMap<ECharacterClass, FCharacterClassInfo> CharacterClassMap;

	/** Select GameplayEffect for SecondaryAttributes (in DA_CharacterClass) */
	UPROPERTY(EditDefaultsOnly, Category = "Attributes Class")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	/** Select GameplayEffect for LifeAttributes (in DA_CharacterClass) */
	UPROPERTY(EditDefaultsOnly, Category = "Attributes Class")
	TSubclassOf<UGameplayEffect> LifeAttributes;

	////****	FUNCTIONS	****////

	FCharacterClassInfo GetCharacterClassInfo(ECharacterClass CharacterClass);
};
