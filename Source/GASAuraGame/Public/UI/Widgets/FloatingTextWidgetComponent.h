// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "FloatingTextWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UFloatingTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFloatingText(float Number, const FString& TextPrefix, const FString& TextSuffix, FSlateColor ColorOverride);
};
