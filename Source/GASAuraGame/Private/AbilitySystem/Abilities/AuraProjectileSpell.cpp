// Super Copyright


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponentBase.h"
#include "Actor/AuraProjectile.h"
#include "GASAuraGame/Public/AuraGameplayTags.h"

void UAuraProjectileSpell::SpawnProjectile(const FVector& TargetLocation)
{
	if (!(GetAvatarActorFromActorInfo() && GetAvatarActorFromActorInfo()->HasAuthority())) return;

	if (ProjectileClass)
	{
		FTransform SpawnTransform;
		FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), FAuraGameplayTags::Get().CombatSocket_Weapon);
		FRotator Rotation = (TargetLocation - SocketLocation).Rotation();
		Rotation.Pitch = 0.f;

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
			
			for (auto& Pair : DamageTypeMap)
			{
				const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
			}

			Projectile->DamageEffectHandle = SpecHandle;
		}

		Projectile->FinishSpawning(SpawnTransform);
	}
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bIsServer = HasAuthority(&ActivationInfo);
	
	if (!bIsServer) return;
}
