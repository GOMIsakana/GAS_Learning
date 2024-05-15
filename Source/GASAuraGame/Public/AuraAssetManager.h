// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:

	static UAuraAssetManager& Get();

protected:
	// 意义是将属性初始化为native属性
	virtual void StartInitialLoading() override;
};
