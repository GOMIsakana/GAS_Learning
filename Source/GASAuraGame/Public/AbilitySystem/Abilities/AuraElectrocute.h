// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraBeamSpell.h"
#include "AuraElectrocute.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UAuraElectrocute : public UAuraBeamSpell
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetDescriptionNextLevel(int32 Level) override;
};
