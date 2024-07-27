// Super Copyright


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponentBase.h"
#include "Actor/AuraProjectile.h"
#include "GASAuraGame/Public/AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void UAuraProjectileSpell::SpawnProjectile(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	if (!(GetAvatarActorFromActorInfo() && GetAvatarActorFromActorInfo()->HasAuthority())) return;

	int32 EffectiveProjectileNum = bOverrideNumProjectileByLevel ? FMath::Max(1, int32(GetAbilityLevel() / 3)) : NumProjectile;
	if (ProjectileClass)
	{
		for (int i = 0; i < EffectiveProjectileNum; i++)
		{
			FTransform SpawnTransform;
			FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
			FRotator Rotation = (TargetLocation - SocketLocation).Rotation();
			if (bOverridePitch)
			{
				Rotation.Pitch = PitchOverride;
			}

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
			OnProjectileSpawnFinishedSignature.Broadcast();
		}
	}
}

void UAuraProjectileSpell::SpawnProjectileWithSpread(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	if (!ProjectileClass) return;

	FVector SocketLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	SocketLocation.Z += 10.f;
	if (GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	}

	FRotator Rotation = (TargetLocation - SocketLocation).Rotation();
	const FVector Forward = Rotation.Vector();
	int32 EffectiveProjectileNum = bOverrideNumProjectileByLevel ? FMath::Max(1, int32(GetAbilityLevel() / 3)) : NumProjectile;
	TArray<FRotator> ProjectileSpawnRotations = UAuraAbilitySystemLibrary::EvenlySpacedRotator(Forward, FVector::UpVector, ProjectileSpread, EffectiveProjectileNum);
	for (FRotator SpawnRots : ProjectileSpawnRotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		if (bOverridePitch)
		{
			SpawnRots.Pitch = PitchOverride;
		}
		SpawnTransform.SetRotation(SpawnRots.Quaternion());

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetAvatarActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		SetupDamageEffectParamsForProjectile(Projectile);

		// Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
		// Projectile->HomingTargetSceneComponent->SetWorldLocation(TargetLocation);
		// Projectile->ProjectileMovementComponent->HomingTargetComponent = Projectile->HomingTargetSceneComponent;

		// Projectile->ProjectileMovementComponent->bIsHomingProjectile = bLaunchHomingProjectile;
		// Projectile->ProjectileMovementComponent->HomingAccelerationMagnitude = FMath::RandRange(HomingAccelerationMin, HomingAccelerationMax);

		Projectile->FinishSpawning(SpawnTransform);
	}
	OnProjectileSpawnFinishedSignature.Broadcast();
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

void UAuraProjectileSpell::SetupDamageEffectParamsForProjectile(AAuraProjectile* InProjectile)
{
	InProjectile->DamageEffectParams = MakeDamageEffectParamasFromClassDefaults();
}
