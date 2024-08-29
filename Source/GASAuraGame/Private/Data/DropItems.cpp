// Super Copyright


#include "Data/DropItems.h"

FItemInfo UDropItems::FindItemInfoByItemTag(const FGameplayTag& ItemTag, bool bLogNotFound) const
{
    TArray<FItemInfo*> OutInfo;
    ItemDataTable.Get()->GetAllRows("", OutInfo);
    for (FItemInfo* Item : OutInfo)
    {
        if (Item->ItemTag == ItemTag)
        {
            return *Item;
        }
    }
    return FItemInfo();
    // return *ItemDataTable.Get()->FindRow<FItemInfo>(ItemTag.GetTagName(), "", bLogNotFound);
}

FDropItem UDropItems::FindDropItemInfoByItemTag(const FGameplayTag& ItemTag, bool bLogNotFound) const
{
    return *ItemDataTable.Get()->FindRow<FDropItem>(ItemTag.GetTagName(), "", bLogNotFound);
}
