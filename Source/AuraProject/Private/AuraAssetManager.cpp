


#include "AuraAssetManager.h"


UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);
	
	auto AssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	
	return *AssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
}
