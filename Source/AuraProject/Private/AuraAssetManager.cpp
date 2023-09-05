


#include "AuraAssetManager.h"
#include "AbilitySystemGlobals.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);
	
	auto AssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	
	return *AssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	//Required for using TargetData.
	UAbilitySystemGlobals::Get().InitGlobalData();
}
