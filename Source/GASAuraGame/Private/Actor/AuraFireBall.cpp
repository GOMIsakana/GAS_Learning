// Super Copyright


#include "Actor/AuraFireBall.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "GameplayCueManager.h"

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();
	StartFlyOutTimeline();
}

void AAuraFireBall::Destroyed()
{
	Super::Super::Destroyed();
}

void AAuraFireBall::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DamageEffectParams.SourceASC == nullptr) return;
	AActor* SourceActor = DamageEffectParams.SourceASC->GetAvatarActor();
	if (SourceActor == nullptr) return;
	if (SourceActor == OtherActor || UAuraAbilitySystemLibrary::ActorIsFriend(SourceActor, OtherActor)) return;
	if (!HasAuthority()) return;

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		DamageEffectParams.TargetASC = TargetASC;
		UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
	}
}

void AAuraFireBall::OnHit()
{
	if (GetOwner())
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FAuraGameplayTags::Get().GameplayCue_FireBlast, CueParams);
	}

	if (LoopSoundComponent)
	{
		LoopSoundComponent->Stop();
	}

	ExplodeDamageParam.DamageOriginLocation = GetActorLocation();
	UAuraAbilitySystemLibrary::ApplyDamageEffect(ExplodeDamageParam);

	bHit = true;
}
