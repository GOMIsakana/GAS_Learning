// Super Copyright


#include "Actor/AuraLightingBall.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayCueFunctionLibrary.h"

AAuraLightingBall::AAuraLightingBall() : Super()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAuraLightingBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAuraLightingBall::RefreshTargetArray()
{
	// 保证Cue的逻辑和这里一样, 即可实现相同的功能

	// 清除不符合条件的老目标
	for (int32 i = 0; i < LightingChainTargets.Num(); i++)
	{
		AActor* Target = LightingChainTargets[i];
		if (Target && IsValid(Target))
		{
			float ToTargetDistance = (GetActorLocation() - Target->GetActorLocation()).Length();
			if (ToTargetDistance > MaxConnectingDistance)
			{
				LightingChainTargets[i] = nullptr;
			}
		}
	}

	// 获取新目标
	UAuraAbilitySystemLibrary::GetLifePlayerWithinRadius(this, Targets, ActorsToIgnore, TargetSearchingDistance, GetActorLocation());
	for (AActor* Target : Targets)
	{
		int32 FirstEmptySystemSlot = -1;
		bool bTargetInMap = false;

		for (int32 i = 0; i < LightingChainTargets.Num(); i++)
		{
			AActor* ChainTarget = LightingChainTargets[i];
			if (FirstEmptySystemSlot == -1 && (!IsValid(ChainTarget) || ChainTarget == nullptr))
			{
				FirstEmptySystemSlot = i;
			}
			if (ChainTarget == Target)
			{
				bTargetInMap = true;
				break;
			}
		}

		// 如果目标先前不在锁定的目标Map中, 而且目标Map中还有空位, 则将当前目标放到第一个找到的空位里
		if (FirstEmptySystemSlot != -1 && !bTargetInMap)
		{
			LightingChainTargets[FirstEmptySystemSlot] = Target;
		}
	}
}

void AAuraLightingBall::DealDamage()
{
	// 查询附近是否有敌人, 如果有不重复的敌人, 则将其添加到列表中
	RefreshTargetArray();
	for (AActor* ChainTarget : LightingChainTargets)
	{
		if (IsValid(ChainTarget) && DamageEffectParams.SourceASC)
		{
			DamageEffectParams.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ChainTarget);
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		else
		{
			ChainTarget = nullptr;
		}
	}
}

void AAuraLightingBall::SetDealingDamage(bool bShouldDealDamage)
{
	if (bShouldDealDamage && !DealDamageTimer.IsValid())
	{
		GetWorldTimerManager().SetTimer(
			DealDamageTimer,
			this,
			&AAuraLightingBall::DealDamage,
			DealDamageFrequency,
			true
		);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(DealDamageTimer);
	}
}

void AAuraLightingBall::StartGameplayCue()
{
	UGameplayCueFunctionLibrary::AddGameplayCueOnActor(GetOwner(), LightingChainsGameplayCueTag, LightingChainsCueParameters);
}

void AAuraLightingBall::EndGameplayCue()
{
	UGameplayCueFunctionLibrary::RemoveGameplayCueOnActor(GetOwner(), LightingChainsGameplayCueTag, LightingChainsCueParameters);
}

void AAuraLightingBall::BeginPlay()
{
	Super::BeginPlay();

	ActorsToIgnore.Add(GetOwner());
	SetDealingDamage(true);
	InitializeLightingChainTargets();
}

void AAuraLightingBall::Destroyed()
{
	Super::Destroyed();
}

void AAuraLightingBall::LifeSpanExpired()
{
	EndGameplayCue();
	OnHit();

	Super::LifeSpanExpired();
}

void AAuraLightingBall::InitializeLightingChainTargets()
{
	LightingChainTargets.Empty(MaxTargetAmount);
	for (int32 i = 0; i < MaxTargetAmount; i++)
	{
		LightingChainTargets.Add(nullptr);
	}
}

