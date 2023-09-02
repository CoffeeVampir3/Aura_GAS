


#include "UI/UIFunctionLibrary.h"

#include "UI/HUD/AuraHUD.h"


AAuraHUD* UUIFunctionLibrary::GetAuraHud(UObject* WorldContextObject)
{
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (const APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			if (AHUD* PlayerHUD = PlayerController->GetHUD())
			{
				AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerHUD);
				return AuraHUD;
			}
		}
	}
	return nullptr;
}
