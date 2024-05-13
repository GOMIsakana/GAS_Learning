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

	virtual void StartInitialLoading() override;
};
