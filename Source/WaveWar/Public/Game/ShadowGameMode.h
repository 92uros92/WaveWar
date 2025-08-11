// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShadowGameMode.generated.h"




UCLASS()
class WAVEWAR_API AShadowGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	virtual void Tick(float DeltaSecond) override;

protected:


	////****	FUNCTIONS	****////


};
