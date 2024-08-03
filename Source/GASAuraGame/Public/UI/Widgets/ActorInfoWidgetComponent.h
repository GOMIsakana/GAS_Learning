// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "ActorInfoWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UActorInfoWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetupActorInfo(const FString& Title, const FString& Content);
};
