// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTags.h"
#include "GameplayEffect.h"
#include "DropItems.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Hand,
	Head,
	Body,
	Leg,
	Feet
};

USTRUCT(BlueprintType)
struct FItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag ItemTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString Description;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 MaxStackAmount = 1;
};

USTRUCT(BlueprintType)
struct FDropItem : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag ItemTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bOverrideDropLevel = true;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 DropLevel = 1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 MaxDropLevel = 1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<EItemType, float> PossibleType;	// 类型, 掉落几率

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<FGameplayTag, float> BaseAttributes;
};

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UDropItems : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> AttributeEquipEffect;
	
	UFUNCTION(BlueprintCallable)
	FItemInfo FindItemInfoByItemTag(const FGameplayTag& ItemTag, bool bLogNotFound = false) const;
	UFUNCTION(BlueprintCallable)
	FDropItem FindDropItemInfoByItemTag(const FGameplayTag& ItemTag, bool bLogNotFound = false) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items Data")
	TObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DropItem Data")
	TObjectPtr<UDataTable> DropItemDataTable;
};
