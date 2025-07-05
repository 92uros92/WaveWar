// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShadowPlayerController.generated.h"




class IEnemyInterface;

UCLASS()
class WAVEWAR_API AShadowPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AShadowPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

private:

	TScriptInterface<IEnemyInterface> ThisActor;

	TScriptInterface<IEnemyInterface> LastActor;

	////****	FUNCTIONS	****////

	void CursorTrace();

protected:

	////****	FUNCTIONS	****////

	virtual void BeginPlay() override;
};
