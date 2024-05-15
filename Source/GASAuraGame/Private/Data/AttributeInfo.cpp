// Super Copyright


#include "Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FAuraAttributeInfo& Info : AttributeInfomation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("无法在 AttributeInfo [%s] 中找到 AttributeTag [%s]"), *GetNameSafe(this), *AttributeTag.ToString())
	}

	return FAuraAttributeInfo();
}
