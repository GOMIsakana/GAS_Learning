// Super Copyright


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SineCount >= SineResetClampInPI * PI)
	{
		SineCount = 0.f;
	}
	if (bShouldRotation)
	{
		DoTickRotation(DeltaTime);
	}
	if (bShouldFloating)
	{
		DoTickFloating(DeltaTime);
	}
	SineCount += DeltaTime;
}

void AAuraEffectActor::DoTickRotation(float DeltaTime)
{
	FRotator RotatorToAdd;
	RotatorToAdd.Yaw = DeltaTime * RotationRate;
	AddActorWorldRotation(RotatorToAdd.Quaternion());
}

void AAuraEffectActor::DoTickFloating(float DeltaTime)
{
	FVector Offset;
	Offset.Z = DeltaTime * FMath::Sin(SineCount * ZAxisFloatingSpeed) * ZAxisFloatingDistance;
	AddActorWorldOffset(Offset);
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

UPARAM(DisplayName = "bApplySuccessful") bool AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	bool bCanApplyEffect = false;
	for (FName Tag : AffectableTag)
	{
		if (TargetActor->ActorHasTag(Tag))
		{
			bCanApplyEffect = true;
			break;
		}
	}

	if (!bCanApplyEffect) return false;
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return false;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle GameEffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*GameEffectSpecHandle.Data.Get());

	bool bStoreEffectHandle = GameEffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite &&
		InfiniteEffectRemovalPolicy != EEffectRemovalPolicy::ERP_DoNotRemove;
	if (bStoreEffectHandle)
	{
		// InfiniteEffectHandlesMap.Add(ActiveEffectHandle, TargetASC);
		InfiniteEffectHandlesMap.Add(TargetASC, ActiveEffectHandle);
	}
	return true;
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::EAP_ApplyOnStartOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::EAP_ApplyOnStartOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::EAP_ApplyOnStartOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::ERP_RemoveOnStartOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;
		TargetASC->RemoveActiveGameplayEffect(InfiniteEffectHandlesMap[TargetASC], InfiniteEffectRemovalStack);
		InfiniteEffectHandlesMap.Remove(TargetASC);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::EAP_ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::EAP_ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::EAP_ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::ERP_RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (IsValid(TargetASC) && InfiniteEffectHandlesMap.Contains(TargetASC) && InfiniteEffectHandlesMap[TargetASC].IsValid())
		{
			TargetASC->RemoveActiveGameplayEffect(InfiniteEffectHandlesMap[TargetASC], InfiniteEffectRemovalStack);
			InfiniteEffectHandlesMap.Remove(TargetASC);
		}
	}
}
