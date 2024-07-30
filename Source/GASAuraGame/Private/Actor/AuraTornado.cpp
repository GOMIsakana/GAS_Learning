// Super Copyright


#include "Actor/AuraTornado.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interaction/CombatInterface.h"

AAuraTornado::AAuraTornado()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AAuraTornado::BeginPlay()
{
	Super::BeginPlay();
	
	ActorToIgnoreDealDamage.Add(GetOwner());
}

void AAuraTornado::Destroyed()
{
	Super::Destroyed();
}

void AAuraTornado::MoveToTarget(float DeltaTime)
{
	FVector TargetLocation;
	TargetLocation = TargetActor ? TargetActor->GetActorLocation() : GetActorLocation();	// 如果有目标就移到目标处, 没目标就原地转
	TargetLocation.Z = GetActorLocation().Z;	// 保证龙卷风只在自己的水平方向移动
	
	FVector ForwardVector = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), TargetLocation);	// 计算当前位置到目标位置的向前向量
	float TargetEuclideanMetric = (TargetLocation - GetActorLocation()).Length();	// 计算当前位置到目标位置的距离
	float EffectiveMoveDistance = FMath::Min(TargetEuclideanMetric, TornadoStepLength);	// 限制步长避免走过头
	TargetLocation = GetActorLocation() + ForwardVector * EffectiveMoveDistance;	// 根据步长计算目标位置
	SetActorLocation(FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, TornadoMovementSpeed));	// 逐渐移至目标位置
}

void AAuraTornado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveToTarget(DeltaTime);
}

void AAuraTornado::SetDealingDamage(bool bShouldDealDamage)
{
	if (bShouldDealDamage && !DealDamageTimer.IsValid())
	{
		GetWorldTimerManager().SetTimer(
			DealDamageTimer,
			this,
			&AAuraTornado::DealDamageAtSelfLocation,
			DealDamageFrequency,
			true
		);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(DealDamageTimer);
	}
}

void AAuraTornado::DealDamageAtSelfLocation()
{
	// 每次造成伤害前更新自己的伤害参数
	DamageEffectParams.DamageOriginLocation = GetActorLocation();
	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
	}
	// 扫描周围敌人的间隔和造成伤害的间隔一致
	if (bFollowNearestEnemy)
	{
		UAuraAbilitySystemLibrary::GetLifePlayerWithinRadius(this, NearestTargets, ActorToIgnoreDealDamage, FollowRadius, GetActorLocation());
		if (NearestTargets.Num() > 0)
		{
			TArray<AActor*> OutNearestTargets;
			UAuraAbilitySystemLibrary::GetClosestTargets(NearestTargets.Num() - 1, NearestTargets, OutNearestTargets, GetActorLocation());
			for (AActor* Actor : OutNearestTargets)
			{
				if (IsValid(Actor) && Actor->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Actor))
				{
					TargetActor = Actor;
					return;
				}
			}
			TargetActor = nullptr;
		}
	}
}

