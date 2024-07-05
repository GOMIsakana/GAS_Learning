// Super Copyright


#include "Data/AbilityInfo.h"
#include "GASAuraGame/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FAuraAbilityInfo& AuraAbilityInfo : AbilityInfomation)
	{
		if (AuraAbilityInfo.AbilityTag == AbilityTag)
		{
			return AuraAbilityInfo;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("无法在技能信息 [%s] 中查询到技能 [%s]"), *GetNameSafe(this), *AbilityTag.ToString());
	}

	return FAuraAbilityInfo();
}
