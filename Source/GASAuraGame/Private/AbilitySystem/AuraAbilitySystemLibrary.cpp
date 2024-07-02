// Super Copyright


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/HUD/AuraHUD.h"
#include "Game/AuraGameModeBase.h"
#include "AuraAbilityTypes.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"

bool UAuraAbilitySystemLibrary::MakeWidgetControllerParamas(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHUD)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutAuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD());
		if (OutAuraHUD)
		{
			AAuraPlayerState* AuraPlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = AuraPlayerState->GetAbilitySystemComponent();
			UAttributeSet* AS = AuraPlayerState->GetAttributeSet();
			OutWCParams.PlayerController = PlayerController;
			OutWCParams.PlayerState = AuraPlayerState;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.AttributeSet = AS;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* AuraHUD = nullptr;
	if (MakeWidgetControllerParamas(WorldContextObject, WCParams, AuraHUD))
	{
		return AuraHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* AuraHUD = nullptr;
	if (MakeWidgetControllerParamas(WorldContextObject, WCParams, AuraHUD))
	{
		return AuraHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* AuraHUD = nullptr;
	if (MakeWidgetControllerParamas(WorldContextObject, WCParams, AuraHUD))
	{
		return AuraHUD->GetSpellMenuWidgetController(WCParams);
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();
	UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo = ClassInfo->GetClassDefaultInfo(CharacterClass);
	
	if (AvatarActor)
	{
		FGameplayEffectContextHandle PrimaryContextHandle = ASC->MakeEffectContext();
		PrimaryContextHandle.AddSourceObject(AvatarActor);
		const FGameplayEffectSpecHandle PrimaryEffectSpec = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*PrimaryEffectSpec.Data.Get());

		FGameplayEffectContextHandle SecondaryContextHandle = ASC->MakeEffectContext();
		SecondaryContextHandle.AddSourceObject(AvatarActor);
		const FGameplayEffectSpecHandle SecondaryEffectSpec = ASC->MakeOutgoingSpec(ClassInfo->SecondaryAttributes, Level, SecondaryContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*SecondaryEffectSpec.Data.Get());

		FGameplayEffectContextHandle VitalContextHandle = ASC->MakeEffectContext();
		VitalContextHandle.AddSourceObject(AvatarActor);
		const FGameplayEffectSpecHandle VitalEffectSpec = ASC->MakeOutgoingSpec(ClassInfo->VitalAttributes, Level, VitalContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*VitalEffectSpec.Data.Get());
	}
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		const FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			const FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetCombatLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AAuraGameModeBase* AuraGamemode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGamemode != nullptr)
	{
		return AuraGamemode->CharacterClassInfo;
	}
	return nullptr;
}

UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	AAuraGameModeBase* AuraGamemode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGamemode != nullptr)
	{
		return AuraGamemode->AbilityInfo;
	}
	return nullptr;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsCriticalHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsDamageValid(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsDamageValid();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float UAuraAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDebuffDamage();
	}
	return -1.f;
}

float UAuraAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDebuffDuration();
	}
	return -1.f;
}

float UAuraAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDebuffFrequency();
	}
	return -1.f;
}

FGameplayTag UAuraAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (AuraEffectContext->GetDamageType())
		{
			return *AuraEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

FVector UAuraAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UAuraAbilitySystemLibrary::GetKnockbackImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetKnockbackImpulse();
	}
	return FVector::ZeroVector;
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsDamageValid(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsDamageValid)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsDamageValid(bInIsDamageValid);
	}
}

void UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDebuff)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsSuccessfulDebuff(bInIsSuccessfulDebuff);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDebuffDamage(InDebuffDamage);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDebuffDuration(InDebuffDuration);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDebuffFrequency(InDebuffFrequency);
	}
}

void UAuraAbilitySystemLibrary::SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		AuraEffectContext->SetDamageType(DamageType);
	}
}

void UAuraAbilitySystemLibrary::SetDeathImpulse(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDeathImpulse(InDeathImpulse);
	}
}

void UAuraAbilitySystemLibrary::SetKnockbackImpulse(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockbackImpulse)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetKnockbackImpulse(InKnockbackImpulse);
	}
}


bool UAuraAbilitySystemLibrary::ActorIsFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayer = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemy = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));

	return bBothArePlayer || bBothAreEnemy;
}

float UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass Class, int32 CombatLevel)
{
	if (UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(Class);
		return ClassDefaultInfo.XPRewardCurve.GetValueAtLevel(CombatLevel);
	}
	return 0.f;
}

FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyDamageEffect(FDamageEffectParams Params)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

	FGameplayEffectContextHandle DamageContextHandle = Params.SourceASC->MakeEffectContext();
	DamageContextHandle.AddSourceObject(Params.SourceASC->GetAvatarActor());
	SetDeathImpulse(DamageContextHandle, Params.DeathImpulse);
	SetKnockbackImpulse(DamageContextHandle, Params.KnockbackImpulse);
	FGameplayEffectSpecHandle DamageSpecHandle = Params.SourceASC->MakeOutgoingSpec(Params.DamageEffectClass, Params.AbilityLevel, DamageContextHandle);

	if (DamageSpecHandle.IsValid())
	{
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Params.DamageType, Params.BaseDamage);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, GameplayTags.Debuff_Data_Chance, Params.DebuffChance);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, GameplayTags.Debuff_Data_Damage, Params.DebuffDamage);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, GameplayTags.Debuff_Data_Duration, Params.DebuffDuration);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, GameplayTags.Debuff_Data_Frequency, Params.DebuffFrequency);
		Params.SourceASC->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), Params.TargetASC);
	}

	return DamageContextHandle;
}

TArray<FRotator> UAuraAbilitySystemLibrary::EvenlySpacedRotator(const FVector& Forward, const FVector& Axis, float Spread, int32 OutputAmount)
{
	TArray<FRotator> RetRotator;
	FVector LeftRotator = Forward;
	if (OutputAmount > 1)
	{
		LeftRotator = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	}
	float DeltaSpread = OutputAmount == 1 ? Spread : Spread / (OutputAmount - 1);
	for (int32 i = 0; i < OutputAmount; i++)
	{
		FVector Direction = LeftRotator.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		RetRotator.Add(Direction.Rotation());
	}
	return RetRotator;
}

TArray<FVector> UAuraAbilitySystemLibrary::EvenlyRotatedVector(const FVector& Forward, const FVector& Axis, float Spread, int32 OutputAmount)
{
	TArray<FVector> RetVector;
	FVector LeftRotator = Forward;
	if (OutputAmount > 1)
	{
		LeftRotator = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	}
	float DeltaSpread = OutputAmount == 1 ? Spread : Spread / (OutputAmount - 1);
	for (int32 i = 0; i < OutputAmount; i++)
	{
		FVector Direction = LeftRotator.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		RetVector.Add(Direction);
	}
	return RetVector;
}

void UAuraAbilitySystemLibrary::GetLifePlayerWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutputOverlappingActor, TArray<AActor*> ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutputOverlappingActor.AddUnique(ICombatInterface::Execute_GetAvatarActor(Overlap.GetActor()));
			}
		}
	}
}

void UAuraAbilitySystemLibrary::GetClosestTargets(int32 MaxTarget, TArray<AActor*> Actors, TArray<AActor*>& OutClosestActors, const FVector& Origin)
{
	struct FSortActorNearbyOrigin
	{
		FSortActorNearbyOrigin(const FVector& InOrigin) : Origin(InOrigin) {};

		FVector Origin;

		FORCEINLINE bool operator()(const AActor& A, const AActor& B) const
		{
			float OriginToA = (A.GetActorLocation() - Origin).Size();
			float OriginToB = (B.GetActorLocation() - Origin).Size();
			return OriginToA < OriginToB;
		}
	};
	OutClosestActors.Reset();
	Actors.Sort(FSortActorNearbyOrigin(Origin));
	int32 InArrayLength = Actors.Num();
	for (int32 i = 0; i < MaxTarget && i < InArrayLength; i++)
	{
		OutClosestActors.AddUnique(Actors[i]);
	}
}
