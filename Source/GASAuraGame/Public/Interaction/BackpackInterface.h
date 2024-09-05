// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTags.h"
#include "Data/DropItems.h"
#include "BackpackInterface.generated.h"

UENUM(BlueprintType)
enum class EEquipState : uint8
{
	Equipped,
	UnEquipped
};

USTRUCT(BlueprintType)
struct FBackpackItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 BackpackSlot = -1; // -1表示不存在于背包中, 默认为0的话会导致0编号上的物品被删除

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag ItemTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 EquipSlot = -1;	// -1表示未装备

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EEquipState EquipState = EEquipState::UnEquipped;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EItemType ItemType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 LevelLimit = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FGameplayTag, float> BaseAttributes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ItemAmount = 0;

	bool operator==(FBackpackItem OtherItem)
	{
		bool bSameBaseAttributes = true;
		if (BaseAttributes.Num() == OtherItem.BaseAttributes.Num())
		{
			TArray<FGameplayTag> AttributeTags;
			BaseAttributes.GetKeys(AttributeTags);
			for (int32 i = 0; i < AttributeTags.Num(); i++)
			{
				FGameplayTag Tag = AttributeTags[i];
				if (!(OtherItem.BaseAttributes.Contains(Tag) && BaseAttributes[Tag] == OtherItem.BaseAttributes[Tag]))
				{
					bSameBaseAttributes = false;
					break;
				}
			}
		}
		else
		{
			bSameBaseAttributes = false;
		}
		// 物品tag、等级限制且属性表一致才认为是同一个物品
		return ItemTag.MatchesTagExact(OtherItem.ItemTag) && LevelLimit == OtherItem.LevelLimit && bSameBaseAttributes;
	}

	bool StructEquals(const FBackpackItem& A, const FBackpackItem& B)
	{
		return FBackpackItem::StaticStruct()->CompareScriptStruct(&A, &B, 0);
	}
};

DECLARE_MULTICAST_DELEGATE_OneParam(FBackpackItemUpdateSignature, FBackpackItem /* 需要更新的物品, 位置信息已包含在里面 */);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBackpackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GASAURAGAME_API IBackpackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ExchangeItem(int32 SourceItemSlot, int32 TargetItemSlot);
	// 这里的排序是排序的数组, 跟读取的没有关系
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SortBackpackItems(bool bAscending = true);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetBackpackSize();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetItemAtBackpackSlot(FBackpackItem& OutItem, int32 InBackpackSlot);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = ())
	void SetItemAtBackpackSlot(FBackpackItem InItem, int32 InBackpackSlot, bool bRemoveInItemSourceSlotItem = true);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetItemAtEquipSlot(FBackpackItem& OutItem, int32 InEquipSlot);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EquipItemToSlot(int32 ToEquipItemBackpackSlot, int32 InEquipSlot);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	// 如果拾取成功, 返回true, 拾取失败返回false
	bool PickupItem(UPARAM(Ref) FBackpackItem& InItem);

	virtual FBackpackItemUpdateSignature& GetBackpackItemUpdateDelegate() = 0;
};