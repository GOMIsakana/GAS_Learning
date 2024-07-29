// Super Copyright


#include "Actor/AuraLightingBall.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayCueFunctionLibrary.h"
#include "Interaction/CombatInterface.h"

AAuraLightingBall::AAuraLightingBall() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAuraLightingBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateLightingChainsEndLocation();
}

void AAuraLightingBall::RefreshTargetArray()
{
	// 获取新目标
	UAuraAbilitySystemLibrary::GetLifePlayerWithinRadius(GetOwner(), Targets, ActorsToIgnore, TargetSearchingDistance, GetActorLocation());
	for (AActor* Target : Targets)
	{
		UNiagaraComponent* FirstEmptySystemSlot = nullptr;
		bool bTargetInMap = false;

		// 目标实现了战斗接口而且没似才会算到目标里面
		if (Target->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Target))
		{
			for (TTuple<UNiagaraComponent*, AActor*>& Pair : LightingChains)
			{
				if (FirstEmptySystemSlot == nullptr && (!IsValid(Pair.Value) || Pair.Value == nullptr))
				{
					FirstEmptySystemSlot = Pair.Key;
				}
				if (Pair.Value == Target)
				{
					bTargetInMap = true;
					break;
				}
			}

			// 如果目标先前不在锁定的目标Map中, 而且目标Map中还有空位, 则将当前目标放到第一个找到的空位里
			if (FirstEmptySystemSlot != nullptr && !bTargetInMap)
			{
				LightingChains[FirstEmptySystemSlot] = Target;
			}
		}
	}
}

void AAuraLightingBall::DealDamage()
{
	// 查询附近是否有敌人, 如果有不重复的敌人, 则将其添加到列表中
	RefreshTargetArray();
	for (TTuple<UNiagaraComponent*, AActor*>& Pair : LightingChains)
	{
		if (IsValid(Pair.Value) && DamageEffectParams.SourceASC)
		{
			DamageEffectParams.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Pair.Value);
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		else
		{
			Pair.Value = nullptr;
		}
	}
}

void AAuraLightingBall::SetDealingDamage(bool bShouldDealDamage)
{
	if (GetOwner() && GetOwner()->GetWorld())
	{
		if (bShouldDealDamage && !DealDamageTimer.IsValid())
		{
			GetOwner()->GetWorld()->GetTimerManager().SetTimer(
				DealDamageTimer,
				this,
				&AAuraLightingBall::DealDamage,
				DealDamageFrequency,
				true
			);
		}
		else
		{
			GetOwner()->GetWorld()->GetTimerManager().ClearTimer(DealDamageTimer);
		}
	}
}

void AAuraLightingBall::BeginPlay()
{
	Super::BeginPlay();

	ActorsToIgnore.Add(GetOwner());
	SetDealingDamage(true);
	InitializeLightingChains();
}

void AAuraLightingBall::Destroyed()
{
	Super::Destroyed();
}

void AAuraLightingBall::LifeSpanExpired()
{
	OnHit();

	Super::LifeSpanExpired();
}

void AAuraLightingBall::InitializeLightingChains()
{
	if (LightingChainSystem)
	{
		for (int32 i = 0; i < MaxTargetAmount; i++)
		{
			UNiagaraComponent* Chain = UNiagaraFunctionLibrary::SpawnSystemAttached(LightingChainSystem, GetRootComponent(), FName(), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTargetIncludingScale, true);
			LightingChains.Add(Chain, nullptr);
		}
	}
}

void AAuraLightingBall::UpdateLightingChainsEndLocation()
{
	// 清除不符合条件的老目标
	for (TTuple<UNiagaraComponent*, AActor*>& Pair : LightingChains)
	{
		if (Pair.Value && IsValid(Pair.Value))
		{
			if (!Pair.Value->Implements<UCombatInterface>() || ICombatInterface::Execute_IsDead(Pair.Value))
			{
				Pair.Value = nullptr;
				continue;
			}
			float ToTargetDistance = (GetActorLocation() - Pair.Value->GetActorLocation()).Length();
			if (ToTargetDistance > MaxConnectingDistance)
			{
				Pair.Value = nullptr;
			}
		}
		else
		{
			Pair.Value = nullptr;
		}
	}
	for (TTuple<UNiagaraComponent*, AActor*>& Pair : LightingChains)
	{
		// 清除目标的逻辑在刷新目标处
		
		// 有目标就连到目标身上
		if (Pair.Value != nullptr && IsValid(Pair.Value))
		{
			Pair.Key->SetNiagaraVariableVec3(FString("Beam End"), Pair.Value->GetActorLocation());
		}
		// 没目标就随便跳
		else
		{
			FVector RandomLocation = GetActorLocation() + UKismetMathLibrary::RandomUnitVector() * TargetSearchingDistance;
			Pair.Key->SetNiagaraVariableVec3(FString("Beam End"), RandomLocation);
		}
	}
}

