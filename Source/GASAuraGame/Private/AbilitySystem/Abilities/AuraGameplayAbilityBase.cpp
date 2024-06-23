// Super Copyright


#include "AbilitySystem/Abilities/AuraGameplayAbilityBase.h"

FString UAuraGameplayAbilityBase::GetDescription(int32 Level)
{
    return FString::Printf(TEXT("<Title>%s</> <Level>Lv. %d</>"), L"描述占位符", 23333);
}

FString UAuraGameplayAbilityBase::GetDescriptionNextLevel(int32 Level)
{
    return FString::Printf(TEXT("<Title>下一级预览</> <Level>Lv. %d</>"), Level);
}

FString UAuraGameplayAbilityBase::GetDescriptionLocked(int32 Level)
{
    return FString::Printf(TEXT("<Title>解锁等级:</> <Level>%d</>"), Level);
}
