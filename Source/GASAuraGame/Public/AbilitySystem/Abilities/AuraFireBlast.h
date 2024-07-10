// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "Actor/AuraFireBall.h"
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

	UFUNCTION(BlueprintCallable)
	TArray<AAuraFireBall*> SpawnFireBalls();

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Combat")
	int32 NumOfProjectile = 12;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AAuraProjectile> ProjectileClass;
};
