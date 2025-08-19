// Uroš Pörš All Rights Reserved


#include "GAS/WW_AssetManager.h"
#include "GAS/WW_GameplayTags.h"




UWW_AssetManager& UWW_AssetManager::Get()
{
	check(GEngine);

	UWW_AssetManager* WWAssetManager = Cast<UWW_AssetManager>(GEngine->AssetManager);
	return *WWAssetManager;
}

void UWW_AssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	/** Starts Initial Loading */
	FWWGameplayTags::InitializeNativeGameplayTags();
}
