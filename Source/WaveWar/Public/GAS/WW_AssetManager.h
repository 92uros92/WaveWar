// Uroš Pörš All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "WW_AssetManager.generated.h"

/**
 * 
 */
UCLASS()
class WAVEWAR_API UWW_AssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:

	static UWW_AssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
};
