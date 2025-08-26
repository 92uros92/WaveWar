// Uroš Pörš All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"



UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};




class WAVEWAR_API IEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetTurnToActor(AActor* InTurnToActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetTurnToActor();
};
