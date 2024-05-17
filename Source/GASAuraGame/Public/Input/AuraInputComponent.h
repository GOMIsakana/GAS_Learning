// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AuraInputConfig.h"
#include "AuraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	template<class UserClass, typename PressedFunctionType, typename ReleasedFunctionType, typename HeldFunctionType>
	UFUNCTION(BlueprintCallable)
	void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFunctionType PressedFunction, ReleasedFunctionType ReleasedFunction, HeldFunctionType HeldFunction);
};

template<class UserClass, typename PressedFunctionType, typename ReleasedFunctionType, typename HeldFunctionType>
inline void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFunctionType PressedFunction, ReleasedFunctionType ReleasedFunction, HeldFunctionType HeldFunction)
{
	if (InputConfig == nullptr) return;

	for (const FAuraInputAction Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction != nullptr && Action.InputTag.IsValid())
		{
			if (PressedFunction)
			{
				// 最后面的Action.InputTag会作为参数传给PressedFunction, 此时即可获取该输入的tag
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunction, Action.InputTag);
			}
			if (ReleasedFunction)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunction, Action.InputTag);
			}
			if (HeldFunction)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered , Object, HeldFunction, Action.InputTag);
			}
		}
	}
}
