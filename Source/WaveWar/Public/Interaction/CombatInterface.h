// Uroš Pörš All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"




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

	virtual int32 GetPlayerLevel();

	virtual FVector GetSocketLocation();

	virtual FVector GetCameraLocation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	virtual void Die() = 0;

};
