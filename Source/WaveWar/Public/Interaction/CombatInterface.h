// Uroš Pörš All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "Data/CharacterClassData.h"
#include "CombatInterface.generated.h"




USTRUCT(BlueprintType)
struct FMontageForAttack
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;
};


UINTERFACE(MinimalAPI, BlueprintType)	
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};


class UAnimMontage;


class WAVEWAR_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:



	////****	FUNCTIONS	****////

	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetSocketLocation(const FGameplayTag& MontageTag);

	virtual FVector GetCameraLocation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	virtual void Die() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsPlayerDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FMontageForAttack> GetAttackMontages();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateTurnToTarget(const FVector& Target);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECharacterClass GetCharacterClass();
};
