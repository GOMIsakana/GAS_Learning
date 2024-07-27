// Super Copyright


#include "Actor/AuraProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "GASAuraGame/GASAuraGame.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>("CapsuleCollision");
	SetRootComponent(Capsule);
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Capsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Capsule->SetCollisionObjectType(ECC_Projectile);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->InitialSpeed = 500.f;
	ProjectileMovementComponent->MaxSpeed = 500.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (bBindCapsuleBeginOverlapDelegate)
	{
		Capsule->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnCapsuleBeginOverlap);
	}

	SetLifeSpan(LifeSpan);
	SetReplicateMovement(true);
	if (HasAuthority() && LoopSound)
	{
		LoopSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopSound, GetRootComponent(), FName(), GetActorLocation(), EAttachLocation::KeepWorldPosition);
	}
}

void AAuraProjectile::Destroyed()
{
	if (!bHit && !HasAuthority())
	{
		OnHit();
	}

	Super::Destroyed();
}

void AAuraProjectile::OnHit()
{
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	}
	if (ImpactEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation(), GetActorRotation());
	}
	if (LoopSoundComponent)
	{
		LoopSoundComponent->Stop();
	}
	bHit = true;
}

void AAuraProjectile::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(DamageEffectParams.SourceASC))
	{
		OnHit();
		Destroy();
	}
	AActor* SourceActor = DamageEffectParams.SourceASC->GetAvatarActor();
	if (SourceActor == nullptr) return;
	if (SourceActor == OtherActor || UAuraAbilitySystemLibrary::ActorIsFriend(SourceActor, OtherActor)) return;
	if (!bHit) OnHit();

	if (HasAuthority())
	{
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

		Destroy();
	}
	else
	{
		bHit = true;
	}
}

