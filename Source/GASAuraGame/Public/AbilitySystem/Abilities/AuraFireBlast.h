// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "Actor/AuraProjectile.h"
#include "AuraFireBlast.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UAuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetDescriptionNextLevel(int32 Level) override;

protected:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Combat")
	int32 NumOfProjectile = 12;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Combat")
	TSubclassOf<AAuraProjectile> ProjectileClass;
};
