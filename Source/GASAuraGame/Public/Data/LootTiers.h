// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "LootTiers.generated.h"

USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LootTiers|Spawning")
	bool bLootItemLevelOverride = true;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LootTiers|Spawning")
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LootTiers|Spawning")
	TObjectPtr<UNiagaraSystem> SpawnSystem;

	UPROPERTY(EditAnywhere, Category = "LootTiers|Spawning")
	float ChanceToSpawn;

	UPROPERTY(EditAnywhere, Category = "LootTiers|Spawning")
	int32 MaxNumToSpawn = 0;
};

/**
 * 
 */
UCLASS()
class GASAURAGAME_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	TArray<FLootItem> GetLootItems();

	UFUNCTION(BlueprintCallable)
	TArray<FLootItem> GetBreakableActorLootItems();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LootTiers|Spawning")
	TArray<FLootItem> LootItemArray;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LootTiers|Spawning")
	TArray<FLootItem> BreakableActorLootItemArray;
};
