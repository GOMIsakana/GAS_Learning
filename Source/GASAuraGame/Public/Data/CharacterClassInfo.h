// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayEffect.h"
#include "CharacterClassInfo.generated.h"

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist,
	Warrior,
	Ranger
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
};

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInfomation;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass) const { return CharacterClassInfomation.FindChecked(CharacterClass); }
};
