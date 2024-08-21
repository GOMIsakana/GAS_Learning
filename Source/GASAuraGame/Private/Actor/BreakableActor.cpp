// Super Copyright


#include "Actor/BreakableActor.h"

ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponentBase>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void ABreakableActor::LoadActor_Implementation()
{
	if (bDead)
	{
		SetActorHiddenInGame(true);
	}
}

void ABreakableActor::HighlightActor_Implementation()
{
}

void ABreakableActor::UnhighlightActor_Implementation()
{
}

void ABreakableActor::SetMoveToDestination_Implementation(FVector& OutDestination)
{
	
}

UAbilitySystemComponent* ABreakableActor::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
}

float ABreakableActor::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageTaken = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	OnDamageDelegate.Broadcast(DamageTaken);

	return DamageTaken;
}

void ABreakableActor::MulticastHandleDeath_Implementation(const FVector& DeathImpluse)
{
	bDead = true;
	OnDeath.Broadcast(this);
}

void ABreakableActor::Die_Implementation(const FVector& DeathImpluse)
{
	MulticastHandleDeath(DeathImpluse);
}

bool ABreakableActor::IsDead_Implementation() const
{
	return bDead;
}

ECharacterClass ABreakableActor::GetCharacterClass_Implementation()
{
	return ECharacterClass::BreakableActor;
}

FOnASCRegistered& ABreakableActor::GetOnASCRegisteredDelegate()
{
	return OnASCRegistered;
}

FOnDeath& ABreakableActor::GetOnDeathDelegate()
{
	return OnDeath;
}

FOnDamageSignature& ABreakableActor::GetOnDamageDelegate()
{
	return OnDamageDelegate;
}

AActor* ABreakableActor::GetAvatarActor_Implementation()
{
	return nullptr;
}
