// Uroš Pörš All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "WW_AIController.generated.h"




class UBlackboardComponent;
class UBehaviorTreeComponent;


UCLASS()
class WAVEWAR_API AWW_AIController : public AAIController
{
	GENERATED_BODY()
	
public:

	AWW_AIController();

protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
