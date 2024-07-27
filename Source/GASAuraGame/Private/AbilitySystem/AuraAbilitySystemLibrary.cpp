// Super Copyright


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/AuraPlayerState.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/HUD/AuraHUD.h"
#include "Game/AuraGameModeBase.h"
#include "AuraAbilityTypes.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GASAuraGame/GASAuraGame.h"

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

void UAuraAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ULoadScreenSaveGame* SaveData)
{
	UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (ASC && ClassInfo)
	{
		FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

		if (ClassInfo->PrimaryAttributes_SetByCaller)
		{
			FGameplayEffectContextHandle PrimaryAttributeContextHandle = ASC->MakeEffectContext();
			PrimaryAttributeContextHandle.AddSourceObject(ASC);
			FGameplayEffectSpecHandle PrimaryAttributeEffectSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->PrimaryAttributes_SetByCaller, 1.f, PrimaryAttributeContextHandle);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PrimaryAttributeEffectSpecHandle, GameplayTags.Attributes_Primary_Strength, SaveData->Strength);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PrimaryAttributeEffectSpecHandle, GameplayTags.Attributes_Primary_Intelligence, SaveData->Intelligence);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PrimaryAttributeEffectSpecHandle, GameplayTags.Attributes_Primary_Resilience, SaveData->Resilience);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PrimaryAttributeEffectSpecHandle, GameplayTags.Attributes_Primary_Vigor, SaveData->Vigor);

			ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeEffectSpecHandle.Data.Get());
		}

		if (ClassInfo->SecondaryAttributes_Infinite)
		{
			FGameplayEffectContextHandle SecondaryAttributeContextHandle = ASC->MakeEffectContext();
			SecondaryAttributeContextHandle.AddSourceObject(ASC);
			FGameplayEffectSpecHandle SecondaryAttributeEffectSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->SecondaryAttributes_Infinite, 1.f, SecondaryAttributeContextHandle);
			ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeEffectSpecHandle.Data.Get());
		}

		if (ClassInfo->VitalAttributes_Infinite)
		{
			FGameplayEffectContextHandle VitalAttributeContextHandle = ASC->MakeEffectContext();
			VitalAttributeContextHandle.AddSourceObject(ASC);
			FGameplayEffectSpecHandle VitalAttributeEffectSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->VitalAttributes_Infinite, 1.f, VitalAttributeContextHandle);
			ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeEffectSpecHandle.Data.Get());
		}
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

ULootTiers* UAuraAbilitySystemLibrary::GetLootTiers(const UObject* WorldContextObject)
{
	AAuraGameModeBase* AuraGamemode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGamemode != nullptr)
	{
		return AuraGamemode->LootTiers;
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

bool UAuraAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsRadialDamage();
	}
	return false;
}

float UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetRadialDamageInnerRadius();
	}
	return 0.0f;
}

float UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetRadialDamageOuterRadius();
	}
	return 0.0f;
}

float UAuraAbilitySystemLibrary::GetMinRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetMinRadialDamage();
	}
	return 0.0f;
}

FVector UAuraAbilitySystemLibrary::GetDamageOriginLocation(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDamageOriginLocation();
	}
	return FVector();
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

void UAuraAbilitySystemLibrary::SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDeathImpulse(InDeathImpulse);
	}
}

void UAuraAbilitySystemLibrary::SetKnockbackImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockbackImpulse)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetKnockbackImpulse(InKnockbackImpulse);
	}
}

void UAuraAbilitySystemLibrary::SetIsRadialDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool InIsRadialDamage)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsRadialDamage(InIsRadialDamage);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageInnerRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InRadialDamageInnerRadius)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetRadialDamageInnerRadius(InRadialDamageInnerRadius);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageOuterRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InRadialDamageOuterRadius)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetRadialDamageOuterRadius(InRadialDamageOuterRadius);
	}
}

void UAuraAbilitySystemLibrary::SetMinRadialDamage(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InMinRadialDamage)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetMinRadialDamage(InMinRadialDamage);
	}
}

void UAuraAbilitySystemLibrary::SetDamageOriginLocation(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDamageOriginLocation)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDamageOriginLocation(InDamageOriginLocation);
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

	SetIsRadialDamage(DamageContextHandle, Params.bIsRadialDamage);
	SetRadialDamageInnerRadius(DamageContextHandle, Params.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(DamageContextHandle, Params.RadialDamageOuterRadius);
	SetMinRadialDamage(DamageContextHandle, Params.MinRadialDamage);
	SetDamageOriginLocation(DamageContextHandle, Params.DamageOriginLocation);

	FVector DamageOriginLocation = Params.DamageOriginLocation;

	if (DamageSpecHandle.IsValid())
	{
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, GameplayTags.Debuff_Data_Chance, Params.DebuffChance);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, GameplayTags.Debuff_Data_Damage, Params.DebuffDamage);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, GameplayTags.Debuff_Data_Duration, Params.DebuffDuration);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, GameplayTags.Debuff_Data_Frequency, Params.DebuffFrequency);
		if (Params.bIsRadialDamage)
		{
			AActor* SourceAvatarActor = Params.SourceASC->GetAvatarActor();
			TArray<AActor*> ActorInRadius;
			TArray<AActor*> ActorToIgnore;
			ActorToIgnore.AddUnique(SourceAvatarActor);
			if (Params.TargetASC)
			{
				ActorToIgnore.AddUnique(Params.TargetASC->GetAvatarActor());
			}
			GetLifePlayerWithinRadius(Params.SourceASC->GetAvatarActor(), ActorInRadius, ActorToIgnore, Params.RadialDamageOuterRadius, Params.DamageOriginLocation);
			for (AActor* Target : ActorInRadius)
			{
				if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
				{
					CombatInterface->GetOnDamageDelegate().Clear();
					FDelegateHandle DamageDelgateHandle = CombatInterface->GetOnDamageDelegate().AddLambda(
						[DamageSpecHandle, Params, Target](float InDamage)
						{
							UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Params.DamageType, InDamage);
							if (Params.KnockbackChance > FMath::RandRange(0.f, 100.f))
							{
								FRotator ImpluseRotation = (Target->GetActorLocation() - Params.SourceASC->GetAvatarActor()->GetActorLocation()).Rotation();
								if (Params.bKnockbackFromOriginLocation)
								{
									ImpluseRotation = (Target->GetActorLocation() - Params.DamageOriginLocation).Rotation();
								}
								ImpluseRotation.Pitch = Params.KnockbackPitch;
								FVector ImpluseVector = ImpluseRotation.Vector() * Params.KnockbackMagnitude;
								if (ACharacter* TargetCharacter = Cast<ACharacter>(Target))
								{
									TargetCharacter->LaunchCharacter(ImpluseVector, true, false);
								}
							}
							Params.SourceASC->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target));
						}
					);
				}
			}
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				SourceAvatarActor,
				Params.BaseDamage,
				Params.MinRadialDamage,
				UAuraAbilitySystemLibrary::GetDamageOriginLocation(DamageContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(DamageContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(DamageContextHandle),
				1.f,
				UDamageType::StaticClass(),
				ActorToIgnore,
				SourceAvatarActor,
				nullptr,
				ECC_Projectile
			);
		}
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Params.DamageType, Params.BaseDamage);
		Params.SourceASC->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), Params.TargetASC);
	}
	
	return DamageContextHandle;
}

TArray<FRotator> UAuraAbilitySystemLibrary::EvenlySpacedRotator(const FVector& Forward, const FVector& Axis, float Spread, int32 OutputAmount)
{
	TArray<FRotator> RetRotator;
	FVector LeftRotator = Forward;
	float DeltaSpread = Spread / OutputAmount;
	if (OutputAmount > 1 && FMath::Fmod(Spread, 360.f) != 0.f)
	{
		LeftRotator = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
		DeltaSpread = Spread / (OutputAmount - 1);
	}
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
	float DeltaSpread = Spread / OutputAmount;
	if (OutputAmount > 1 && FMath::Fmod(Spread, 360.f) != 0.f)
	{
		LeftRotator = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
		DeltaSpread = Spread / (OutputAmount - 1);
	}
	for (int32 i = 0; i < OutputAmount; i++)
	{
		FVector Direction = LeftRotator.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		RetVector.Add(Direction);
	}
	return RetVector;
}

TArray<FVector> UAuraAbilitySystemLibrary::GetRandomPointInRadiusSafe(const UObject* WorldContextObject, const FVector& CenterPointPos, int32 NumOfPoints, float Radius, bool bSortByDistance)
{
	TArray<FVector> Points;

	TArray<AActor*> ActorsInRadius;
	GetLifePlayerWithinRadius(WorldContextObject, ActorsInRadius, TArray<AActor*>(), Radius, CenterPointPos);

	// 生成随机点
	for (int i = 0; i < NumOfPoints; i++)
	{
		// 获取随机点位
		FVector Point = CenterPointPos + UKismetMathLibrary::RandomUnitVector() * Radius;
		// 从上到下进行射线检测
		FHitResult HitResult;
		FVector TraceHigherLocation = FVector(Point.X, Point.Y, Point.Z + 500.f);
		FVector TraceLowerLocation = FVector(Point.X, Point.Y, Point.Z - 500.f);
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(ActorsInRadius);
		WorldContextObject->GetWorld()->LineTraceSingleByProfile(HitResult, TraceHigherLocation, TraceLowerLocation, FName("BlockAll"), QueryParams);
		// 将范围内第一个碰撞点放入数组中
		if (HitResult.bBlockingHit)
		{
			Points.Add(HitResult.ImpactPoint);
		}
		else
		{
			Points.Add(Point);
		}
	}

	// 升序排序
	if (bSortByDistance)
	{
		struct FSortByDistance
		{
			FSortByDistance(const FVector& InOriginPos) : OriginPos(InOriginPos) {};

			FVector OriginPos;

			FORCEINLINE bool operator()(const FVector& A, const FVector& B) const
			{
				float DistanceToA = (A - OriginPos).Size();
				float DistanceToB = (B - OriginPos).Size();
				return DistanceToA < DistanceToB;
			}
		};

		Points.Sort(FSortByDistance(CenterPointPos));
	}

	return Points;
}

void UAuraAbilitySystemLibrary::GetLifePlayerWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutputOverlappingActor, TArray<AActor*> ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	OutputOverlappingActor.Empty();
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
			float OriginToA = (A.GetActorLocation() - Origin).Length();
			float OriginToB = (B.GetActorLocation() - Origin).Length();
			return OriginToA < OriginToB;
		}
	};
	OutClosestActors.Reset();
	int32 InArrayLength = Actors.Num();
	Actors.Sort(FSortActorNearbyOrigin(Origin));
	for (int32 i = 0; i < MaxTarget && i < InArrayLength; i++)
	{
		OutClosestActors.AddUnique(Actors[i]);
	}
}
