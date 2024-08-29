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
	int32 BackpackSlot;

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
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FBackpackItemMovedSignature, int32 /* 起点位置 */, int32 /* 终点位置 */);

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
	UFUNCTION(BlueprintNativeEvent)
	void ExchangeItem(int32 SourceItemSlot, int32 TargetItemSlot);
	// 这里的排序是排序的数组, 跟读取的没有关系
	UFUNCTION(BlueprintNativeEvent)
	void SortBackpackItems(bool bAscending = true);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetBackpackSize();

	UFUNCTION(BlueprintNativeEvent)
	void GetItemAtBackpackSlot(FBackpackItem& OutItem, int32 BackpackSlot);
	UFUNCTION(BlueprintNativeEvent)
	void SetItemAtBackpackSlot(FBackpackItem& InItem, int32 BackpackSlot);

	UFUNCTION(BlueprintNativeEvent)
	void GetItemAtEquipSlot(FBackpackItem& OutItem, int32 EquipSlot);
	UFUNCTION(BlueprintNativeEvent)
	void EquipItemToSlot(int32 ToEquipItemBackpackSlot, int32 EquipSlot);

	UFUNCTION(BlueprintNativeEvent)
	// 如果拾取成功, 返回true, 拾取失败返回false
	bool PickupItem(FBackpackItem& InItem);

	virtual FBackpackItemMovedSignature& GetBackpackItemMovedDelegate() = 0;
};