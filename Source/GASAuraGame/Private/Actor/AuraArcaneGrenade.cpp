// Super Copyright


#include "Actor/AuraArcaneGrenade.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

void AAuraArcaneGrenade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraArcaneGrenade, RemainBounceTimes);
}

void AAuraArcaneGrenade::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &AAuraArcaneGrenade::OnProjectileBounce);
}

void AAuraArcaneGrenade::Destroyed()
{
	if (LoopSoundComponent)
	{
		LoopSoundComponent->Stop();
	}

	Super::Destroyed();
}

void AAuraArcaneGrenade::OnProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	if (!HasAuthority()) return;
	if (RemainBounceTimes > 0 && DamageEffectParams.SourceASC)
	{
		// 通知GC播放特效
		FGameplayCueParameters CueParams;
		CueParams.Location = ImpactResult.ImpactPoint;
		DamageEffectParams.SourceASC->ExecuteGameplayCue(FAuraGameplayTags::Get().GameplayCue_ArcaneBlast, CueParams);
		// 造成范围伤害
		DamageEffectParams.DamageOriginLocation = ImpactResult.ImpactPoint;
		UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		RemainBounceTimes--;
	}

	if (!RemainBounceTimes)
	{
		Destroy();
	}
}