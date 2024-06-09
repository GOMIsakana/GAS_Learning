// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbilityBase.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UAuraSummonAbility : public UAuraGameplayAbilityBase
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();

	UFUNCTION(BlueprintPure)
	TSubclassOf<APawn> GetRandomMinionClass();

	UPROPERTY(EditDefaultsOnly, Category = "Summoing")
	int32 NumberOfMinions = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Summoing")
	TArray<TSubclassOf<APawn>> MinionsClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Summoing")
	float MinSpawnDistance = 150.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summoing")
	float MaxSpawnDistance = 350.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summoing")
	float SpawnPositionRotator = 90.f;
};
