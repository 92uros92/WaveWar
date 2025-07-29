// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

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
