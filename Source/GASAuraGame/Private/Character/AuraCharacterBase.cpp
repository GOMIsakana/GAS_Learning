// Super Copyright


#include "Character/AuraCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GASAuraGame/GASAuraGame.h"
#include "AuraGameplayTags.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>(TEXT("燃烧Debuff粒子效果"));
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = FAuraGameplayTags::Get().Debuff_Type_Burn;

	StunDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>(TEXT("眩晕Debuff粒子效果"));
	StunDebuffComponent->SetupAttachment(GetRootComponent());
	StunDebuffComponent->DebuffTag = FAuraGameplayTags::Get().Debuff_Type_Stun;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("武器"));
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Weapon->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	EffectAttachComponent = CreateDefaultSubobject<USceneComponent>(TEXT("被动技能效果附加根节点"));
	EffectAttachComponent->SetupAttachment(GetRootComponent());

	HaloOfProtectionComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>(TEXT("保护光环技能粒子效果"));
	HaloOfProtectionComponent->SetupAttachment(EffectAttachComponent);

	LifeSiphonComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>(TEXT("生命虹吸技能粒子效果"));
	LifeSiphonComponent->SetupAttachment(EffectAttachComponent);

	ManaSiphonComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>(TEXT("法力虹吸技能粒子效果"));
	ManaSiphonComponent->SetupAttachment(EffectAttachComponent);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraCharacterBase, bIsStunned);
	DOREPLIFETIME(AAuraCharacterBase, bIsBeingShock);
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon))
	{
		return Weapon == nullptr ? FVector() : Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Tail))
	{
		return GetMesh()->GetSocketLocation(TailSocketName);
	}
	return FVector();
}

USkeletalMeshComponent* AAuraCharacterBase::GetWeapon_Implementation()
{
	return Weapon;
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AAuraCharacterBase::GetAvatarActor_Implementation()
{
	return this;
}

UNiagaraSystem* AAuraCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontage AAuraCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage Montage : AttackMontages)
	{
		if (Montage.MontageTag == MontageTag)
		{
			return Montage;
		}
	}
	return FTaggedMontage();
}

int32 AAuraCharacterBase::GetMinionsCount_Implementation()
{
	return MinionsCount;
}

void AAuraCharacterBase::SetMinionsCount_Implementation(int32 InMinionsCount)
{
	MinionsCount = InMinionsCount;
}

ECharacterClass AAuraCharacterBase::GetCharacterClass_Implementation()
{
	return ECharacterClass::Elementalist;
}

FOnASCRegistered& AAuraCharacterBase::GetOnASCRegisteredDelegate()
{
	return OnASCRegistered;
}

FOnDeath& AAuraCharacterBase::GetOnDeathDelegate()
{
	return OnDeath;
}

bool AAuraCharacterBase::IsBeingShock_Implementation()
{
	return bIsBeingShock;
}

void AAuraCharacterBase::SetIsBeingShock_Implementation(bool bInIsBeingShock)
{
	bIsBeingShock = bInIsBeingShock;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

TArray<FTaggedMontage> AAuraCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

void AAuraCharacterBase::Die(const FVector& DeathImpulse)
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath(DeathImpulse);
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	bDead = true;

	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	FVector ImpulseToApply = DeathImpulse;
	ImpulseToApply.Z = 0.f;

	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->AddImpulse(ImpulseToApply * .1f, NAME_None, true);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->AddImpulse(ImpulseToApply, NAME_None, true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OnDeath.Broadcast(this);

	Dissolve();
}

void AAuraCharacterBase::OnStunTagChanged(FGameplayTag ReceivedTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;
	bool bShouldStopMove = bHitReacting || bIsStunned;
	GetCharacterMovement()->MaxWalkSpeed = bShouldStopMove ? 0.f : BaseWalkSpeed;
}

void AAuraCharacterBase::OnRep_IsStunned()
{
}

void AAuraCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	EffectAttachComponent->SetWorldRotation(FRotator::ZeroRotator);
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	if (!IsValid(GetAbilitySystemComponent()) || GameplayEffectClass == nullptr) return;
	FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpceHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, EffectContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpceHandle.Data.Get(), GetAbilitySystemComponent());
}

void AAuraCharacterBase::InitializeDefaultAttribute() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributesClass, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributesClass, 1.f);
	ApplyEffectToSelf(DefaultInitializeVitalAttributesClass, 1.f);
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	if (!HasAuthority()) return;

	UAuraAbilitySystemComponentBase* ASC = Cast<UAuraAbilitySystemComponentBase>(AbilitySystemComponent);
	ASC->AddCharacterAbilities(StartupAbilities);
	ASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

void AAuraCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartDissolveTimeline(DynamicMatInst);	// 时间线交给蓝图实现
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatInst);
		StartWeaponDissolveTimeline(DynamicMatInst);	// 时间线交给蓝图实现
	}
}

