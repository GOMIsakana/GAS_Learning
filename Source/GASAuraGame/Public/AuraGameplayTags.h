// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Aura Gameplay Tags，
 * 
 * 
 */

struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Secondary_PhysicalDamage;
protected:

private:
	// 这个的意义是为了让上面的Get有意义，上面的Get一般会以 FAuraGameplayTags::Get() 的形式调用，此时获取到的就是这个在结构体内创建的变量
	static FAuraGameplayTags GameplayTags;
};
