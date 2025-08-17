// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

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
