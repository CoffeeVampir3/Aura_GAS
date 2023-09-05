


#include "UI/UIFunctionLibrary.h"
#include "Subsystems/WidgetControllerSubsystem.h"

UWidgetControllerSubsystem* UUIFunctionLibrary::GetWidgetControllerSubsystem(UObject* WorldContextObject)
{
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		return World->GetSubsystem<UWidgetControllerSubsystem>();
	}
	return nullptr;
}
