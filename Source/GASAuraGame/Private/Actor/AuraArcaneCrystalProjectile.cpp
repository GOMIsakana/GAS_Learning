// Super Copyright


#include "Actor/AuraArcaneCrystalProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AuraGameplayTags.h"

void AAuraArcaneCrystalProjectile::StartOverlapCooldown()
{
	bCanCauseOverlapDamage = false;
	GetWorldTimerManager().SetTimer(
		OverlapCooldownTimer,
		this,
		&AAuraArcaneCrystalProjectile::OverlapCooldownTimerFinish,
		OverlapCooldown,
		false
	);
}

void AAuraArcaneCrystalProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &AAuraArcaneCrystalProjectile::OnProjectileBounce);
}

void AAuraArcaneCrystalProjectile::Destroyed()
{
	OnHit();

	Super::Destroyed();
}

void AAuraArcaneCrystalProjectile::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(DamageEffectParams.SourceASC))
	{
		OnHit();
		Destroy();
	}

	AActor* SourceActor = DamageEffectParams.SourceASC->GetAvatarActor();
	if (SourceActor == nullptr) return;
	if (SourceActor == OtherActor || UAuraAbilitySystemLibrary::ActorIsFriend(SourceActor, OtherActor)) return;
	// if (!bHit) OnHit();

	if (HasAuthority() && bCanCauseOverlapDamage)
	{
		StartOverlapCooldown();
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;

			const bool bKnockback = DamageEffectParams.KnockbackChance >= FMath::RandRange(0.f, 100.f);
			if (bKnockback)
			{
				FRotator TargetRotation = OtherActor->GetActorRotation();
				TargetRotation.Pitch = 45.f;

				const FVector KnockbackImpulse = TargetRotation.Vector() * DamageEffectParams.KnockbackMagnitude;
				DamageEffectParams.KnockbackImpulse = KnockbackImpulse;
			}

			DamageEffectParams.TargetASC = TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}

		DamageEffectParams.BaseDamage *= (1 + OverlapIncreaseDamageMagnitude);
	}
}

void AAuraArcaneCrystalProjectile::OnProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	if (!HasAuthority()) return;
	if (DamageEffectParams.SourceASC)
	{
		FGameplayCueParameters Params;
		Params.Location = ImpactResult.ImpactPoint;
		DamageEffectParams.SourceASC->ExecuteGameplayCue(FAuraGameplayTags::Get().GameplayCue_ArcaneStorm, Params);
	}
	DamageEffectParams.BaseDamage *= (1 + OverlapIncreaseDamageMagnitude);
}

void AAuraArcaneCrystalProjectile::OverlapCooldownTimerFinish()
{
	bCanCauseOverlapDamage = true;
}
