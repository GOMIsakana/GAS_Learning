// Super Copyright


#include "Data/LootTiers.h"

TArray<FLootItem> ULootTiers::GetLootItems()
{
	TArray<FLootItem> RetLootItems;

	for (FLootItem& Item : LootItemArray)
	{
		for (int i = 0; i < Item.MaxNumToSpawn; i++)
		{
			if (Item.ChanceToSpawn > FMath::RandRange(1.f, 100.f))
			{
				FLootItem NewItem;
				NewItem.bLootItemLevelOverride = Item.bLootItemLevelOverride;
				NewItem.ItemClass = Item.ItemClass;
				NewItem.SpawnSystem = Item.SpawnSystem;
				RetLootItems.Add(NewItem);
			}
		}
	}
	
	return RetLootItems;
}

TArray<FLootItem> ULootTiers::GetBreakableActorLootItems()
{
	TArray<FLootItem> RetLootItems;

	for (FLootItem& Item : BreakableActorLootItemArray)
	{
		for (int i = 0; i < Item.MaxNumToSpawn; i++)
		{
			if (Item.ChanceToSpawn > FMath::RandRange(1.f, 100.f))
			{
				FLootItem NewItem;
				NewItem.bLootItemLevelOverride = Item.bLootItemLevelOverride;
				NewItem.ItemClass = Item.ItemClass;
				NewItem.SpawnSystem = Item.SpawnSystem;
				RetLootItems.Add(NewItem);
			}
		}
	}

	return RetLootItems;
}
