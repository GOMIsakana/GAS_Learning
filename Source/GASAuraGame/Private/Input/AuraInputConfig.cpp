// Super Copyright


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& Tag, bool bLogNotFound) const
{
	for (const FAuraInputAction& SingalAction : AbilityInputActions)
	{
		if (SingalAction.InputAction && SingalAction.InputTag == Tag)
		{
			return SingalAction.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("无法在技能动作列表 [%s] 中找到带有 [%s] Tag的动作"), *GetNameSafe(this), *Tag.ToString())
	}

	return nullptr;
}
