// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "Delegates/DelegateInstancesImpl.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT(BlueprintType)
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {}
	FEffectProperties(UAbilitySystemComponent* SASC, AActor* SAA, AController* SController, ACharacter* SCharacter,
		UAbilitySystemComponent* TASC, AActor* TAA, AController* TController, ACharacter* TCharacter, FGameplayEffectContextHandle ECH) :
		SourceASC(SASC), SourceAvatarActor(SAA), SourceController(SController), SourceCharacter(SCharacter),
		TargetASC(TASC), TargetAvatarActor(TAA), TargetController(TController), TargetCharacter(TCharacter), EffectContextHandle(ECH) {}

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;
	UPROPERTY()
	AController* SourceController = nullptr;
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;
	UPROPERTY()
	AController* TargetController = nullptr;
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;

	FGameplayEffectContextHandle EffectContextHandle;
};

//typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;
// 
// 把函数的地址作为指针返回出去。template比typedef更通用
// 
// 不知什么原因导致继承模板无法使用, 只能直接用函数指针了
// template<class T>
// using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UAuraAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue);
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data);

	TMap<FGameplayTag, FGameplayAttribute(*)()> TagsToAttributes;

	/* 在.h文件中定义函数指针
	TStaticFuncPtr<float(int32, float, int32)> AnyFunctionPtr;
	static float AnyFunction(int32 integer1, float f1, int32 integer2);

	在.cpp文件中，将函数指针和函数绑定
	AnyFunctionPtr = AnyFunction;
	float res = AnyFunctionPtr(values...)
	*/

	/*
	* 次要属性
	*/

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attribute")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalDamage, Category = "Secondary Attribute")
	FGameplayAttributeData PhysicalDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalDamage);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Defense, Category = "Secondary Attribute")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Defense);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DefensePenetration, Category = "Secondary Attribute")
	FGameplayAttributeData DefensePenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, DefensePenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicalDamage, Category = "Secondary Attribute")
	FGameplayAttributeData MagicalDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MagicalDamage);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resist, Category = "Secondary Attribute")
	FGameplayAttributeData Resist;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resist);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistPenetration, Category = "Secondary Attribute")
	FGameplayAttributeData ResistPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ResistPenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attribute")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attribute")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritChance, Category = "Secondary Attribute")
	FGameplayAttributeData CritChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CritChance);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritDamage, Category = "Secondary Attribute")
	FGameplayAttributeData CritDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CritDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resist_Fire, Category = "Secondary Attribute")
	FGameplayAttributeData Resist_Fire;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resist_Fire);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resist_Lighting, Category = "Secondary Attribute")
	FGameplayAttributeData Resist_Lighting;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resist_Lighting);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resist_Arcane, Category = "Secondary Attribute")
	FGameplayAttributeData Resist_Arcane;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resist_Arcane);

	/*
	* 主要属性
	*/

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attribute")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attribute")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attribute")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attribute")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);

	/*
	* 关键属性
	*/

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attribute")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

	/*
	* 元属性	元属性是在本地计算的属性，作为一个计算结果影响其他属性
	*/

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attribute")
	FGameplayAttributeData InComingDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, InComingDamage);

	/* 关键属性 */

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	/* 主要属性 */

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	/* 次要属性 */

	UFUNCTION()
	void OnRep_PhysicalDamage(const FGameplayAttributeData& OldPhysicalDamage) const;

	UFUNCTION()
	void OnRep_Defense(const FGameplayAttributeData& OldDefense) const;

	UFUNCTION()
	void OnRep_DefensePenetration(const FGameplayAttributeData& OldDefensePenetration) const;

	UFUNCTION()
	void OnRep_MagicalDamage(const FGameplayAttributeData& OldMagicalDamage) const;

	UFUNCTION()
	void OnRep_Resist(const FGameplayAttributeData& OldResist) const;

	UFUNCTION()
	void OnRep_ResistPenetration(const FGameplayAttributeData& OldResistPenetration) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegenration) const;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

	UFUNCTION()
	void OnRep_CritChance(const FGameplayAttributeData& OldCritChance) const;

	UFUNCTION()
	void OnRep_CritDamage(const FGameplayAttributeData& OldCritDamage) const;

	UFUNCTION()
	void OnRep_Resist_Fire(const FGameplayAttributeData& OldResist_Fire) const;
	UFUNCTION()
	void OnRep_Resist_Lighting(const FGameplayAttributeData& OldResist_Lighting) const;
	UFUNCTION()
	void OnRep_Resist_Arcane(const FGameplayAttributeData& OldResist_Arcane) const;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
	void ShowFloatingText(const FEffectProperties& Props, float DamageAmount, bool bCriticalHit, bool bDamageValid);
};
