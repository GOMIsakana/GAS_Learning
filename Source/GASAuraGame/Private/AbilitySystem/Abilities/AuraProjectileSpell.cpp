// Super Copyright


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponentBase.h"
#include "Actor/AuraProjectile.h"
#include "GASAuraGame/Public/AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UAuraProjectileSpell::SpawnProjectile(const FVector& TargetLocation, const FGameplayTag& SocketTag)
{
	if (!(GetAvatarActorFromActorInfo() && GetAvatarActorFromActorInfo()->HasAuthority())) return;

	if (ProjectileClass)
	{
		for (int i = 0; i < NumProjectile; i++)
		{
			FTransform SpawnTransform;
			FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
			FRotator Rotation = (TargetLocation - SocketLocation).Rotation();
			// Rotation.Pitch = 0.f;

			SpawnTransform.SetLocation(SocketLocation);
			SpawnTransform.SetRotation(Rotation.Quaternion());

			AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
				ProjectileClass,
				SpawnTransform,
				GetOwningActorFromActorInfo(),
				Cast<APawn>(GetOwningActorFromActorInfo()),
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);


			if (DamageEffectClass)
			{
				const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
				const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());

				const float ScaledDamage = ScalableDamage.GetValueAtLevel(GetAbilityLevel());
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageType, ScaledDamage);

				FDamageEffectParams DamageParams = MakeDamageEffectParamasFromClassDefaults();

				Projectile->DamageEffectParams = DamageParams;
			}

			Projectile->FinishSpawning(SpawnTransform);
		}
	}
}

FString UAuraProjectileSpell::GetDescription(int32 Level)
{
	return FString();
}

FString UAuraProjectileSpell::GetDescriptionNextLevel(int32 Level)
{
	return FString();
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bIsServer = HasAuthority(&ActivationInfo);
	
	if (!bIsServer) return;
}
