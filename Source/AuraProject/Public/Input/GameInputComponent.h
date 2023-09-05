

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameInputConfiguration.h"
#include "GameInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURAPROJECT_API UGameInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
	
public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(
		const UGameInputConfiguration* InputConfigs,
		UserClass* Object,
		PressedFuncType PressedFunc,
		ReleasedFuncType ReleasedFunc,
		HeldFuncType HeldFunc);
	
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UGameInputComponent::BindAbilityActions(const UGameInputConfiguration* InputConfigs, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfigs);

	for (const auto [InputAction, InputTag] : InputConfigs->AbilityInputActions)
	{
		if(InputAction && InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(InputAction, ETriggerEvent::Started, Object, PressedFunc, InputTag);
			}
			if (ReleasedFunc)
            {
            	BindAction(InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputTag);
            }
			if (HeldFunc)
			{
				BindAction(InputAction, ETriggerEvent::Triggered, Object, HeldFunc, InputTag);
			}
		}
	}
}
