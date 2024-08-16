// Super Copyright


#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "AuraGameplayTags.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameplayEffect.h"
#include "AuraAbilityTypes.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);
	
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_PhysicalDamage, GetPhysicalDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Defense, GetDefenseAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_DefensePenetration, GetDefensePenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MagicalDamage, GetMagicalDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Resist, GetResistAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ResistPenetration, GetResistPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CritChance, GetCritChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CritDamage, GetCritDamageAttribute);

	TagsToAttributes.Add(GameplayTags.Attributes_Resist_Fire, GetResist_FireAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resist_Lighting, GetResist_LightingAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resist_Arcane, GetResist_ArcaneAttribute);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Defense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, DefensePenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MagicalDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resist, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ResistPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CritChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CritDamage, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resist_Fire, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resist_Lighting, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resist_Arcane, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	bool bTargetCharacterValid = Props.TargetCharacter && Props.TargetCharacter->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Props.TargetCharacter);
	bool bTargetAvatarActorValid = Props.TargetAvatarActor && Props.TargetAvatarActor->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Props.TargetAvatarActor);
	if (!(bTargetCharacterValid || bTargetAvatarActorValid)) return;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetInComingDamageAttribute())
	{
		HandleDamage(Props);
	}
	if (Data.EvaluatedData.Attribute == GetInComingXPAttribute())
	{
		HandleXP(Props);
	}
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() || Attribute == GetMaxManaAttribute())
	{
		HandleLevelRecovery();
	}
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.GetEffectContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (APawn* SourcePawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = SourcePawn->GetController();
			}
		}
		if (Props.SourceController != nullptr)
		{
			Props.SourceCharacter = Props.SourceController->GetCharacter();
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UAuraAttributeSet::ShowFloatingText(const FEffectProperties& Props, float DamageAmount, bool bCriticalHit, bool bDamageValid)
{

	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(Props.SourceController))
		{
			PC->ClientShowDamageNumber(DamageAmount, Props.TargetCharacter, bCriticalHit, bDamageValid);
		}
		if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(Props.TargetController))
		{
			PC->ClientShowDamageNumber(DamageAmount, Props.TargetCharacter, bCriticalHit, bDamageValid);
		}
	}
}

void UAuraAttributeSet::SendXPEvent(FEffectProperties& Props)
{
	
	if (Props.TargetCharacter && Props.TargetCharacter->Implements<UCombatInterface>())
	{
		const float XPReward = UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(this, ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter), ICombatInterface::Execute_GetCombatLevel(Props.TargetCharacter));
		FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXP;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceAvatarActor, GameplayTags.Attributes_Meta_IncomingXP, Payload);
	}
}

void UAuraAttributeSet::HandleDamage(FEffectProperties Props)
{
	const float LocalInComingDamage = GetInComingDamage();
	SetInComingDamage(0.f);
	if (LocalInComingDamage >= 0.f)
	{
		const float NewHealth = GetHealth() - LocalInComingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		const bool bFatal = NewHealth <= 0.f;
		if (bFatal)
		{
			FVector DeathImpulse = UAuraAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle);
			if (Props.TargetAvatarActor && Props.TargetAvatarActor->Implements<UCombatInterface>())
			{
				ICombatInterface::Execute_Die(Props.TargetAvatarActor, DeathImpulse);
			}

			SendXPEvent(Props);
		}
		else
		{
			// 受击硬直
			FGameplayTagContainer Tags;
			Tags.AddTag(FAuraGameplayTags::Get().Abilities_HitReact);
			bool bActivateHitReact = UAuraAbilitySystemLibrary::IsActivateHitReact(Props.EffectContextHandle);
			if (bActivateHitReact && Props.TargetCharacter && Props.TargetCharacter->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsBeingShock(Props.TargetCharacter))
			{
				Props.TargetASC->TryActivateAbilitiesByTag(Tags);
			}

			if (UAuraAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
			{
				HandleDebuff(Props);
			}

			// 击退
			FVector KnockbackImpulse = UAuraAbilitySystemLibrary::GetKnockbackImpulse(Props.EffectContextHandle);
			if (Props.TargetCharacter && !KnockbackImpulse.IsNearlyZero(1.f))
			{
				Props.TargetCharacter->LaunchCharacter(KnockbackImpulse, true, false);
			}
		}

		const bool bCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
		const bool bDamageValid = UAuraAbilitySystemLibrary::IsDamageValid(Props.EffectContextHandle);
		ShowFloatingText(Props, LocalInComingDamage, bCriticalHit, bDamageValid);
	}
}

void UAuraAttributeSet::HandleXP(FEffectProperties Props)
{
	const float LocalIncomingXP = GetInComingXP();
	SetInComingXP(0.f);

	if (Props.SourceCharacter->Implements<UPlayerInterface>() && Props.SourceCharacter->Implements<UCombatInterface>())
	{
		int32 CurrentLevel = ICombatInterface::Execute_GetCombatLevel(Props.SourceCharacter);
		float CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceCharacter);

		int32 NextLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter, CurrentXP + LocalIncomingXP);
		int32 NumOfLevelUp = NextLevel - CurrentLevel;
		if (NumOfLevelUp > 0)
		{
			for (int32 i = 0; i < NumOfLevelUp; i++)
			{
				const int32 AttributePointsReward = IPlayerInterface::Execute_GetAttributePointsReward(Props.SourceCharacter, CurrentLevel);
				const int32 SpellPointsReward = IPlayerInterface::Execute_GetSpellPointsReward(Props.SourceCharacter, CurrentLevel);

				IPlayerInterface::Execute_AddToAttributePointsReward(Props.SourceCharacter, AttributePointsReward);
				IPlayerInterface::Execute_AddToSpellPointsReward(Props.SourceCharacter, SpellPointsReward);
			}

			bTopOffHealth = true;
			bTopOffMana = true;

			IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
			IPlayerInterface::Execute_AddToCombatLevel(Props.SourceCharacter, NumOfLevelUp);
			HandleLevelRecovery();
		}

		IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalIncomingXP);
	}
}

void UAuraAttributeSet::HandleDebuff(FEffectProperties Props)
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	FGameplayEffectContextHandle DebuffContextHandle = Props.SourceASC->MakeEffectContext();
	DebuffContextHandle.AddSourceObject(Props.SourceAvatarActor);

	const FGameplayTag DamageType = UAuraAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);
	const float DebuffDamage = UAuraAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);
	const float DebuffDuration = UAuraAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);
	const float DebuffFrequency = UAuraAbilitySystemLibrary::GetDebuffFrequency(Props.EffectContextHandle);

	if (!GameplayTags.DamageTypeToDebuff.Contains(DamageType)) return;

	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString());
	UGameplayEffect* DebuffEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));

	DebuffEffect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DebuffEffect->Period = DebuffFrequency;
	DebuffEffect->DurationMagnitude = FScalableFloat(DebuffDuration);

	FGameplayTag DebuffTag = GameplayTags.DamageTypeToDebuff[DamageType];

	DebuffEffect->InheritableOwnedTagsContainer.AddTag(DebuffTag);

	if (DebuffTag.MatchesTagExact(GameplayTags.Debuff_Type_Stun))
	{
		DebuffEffect->InheritableOwnedTagsContainer.AddTag(GameplayTags.Player_Block_InputHeld);
		DebuffEffect->InheritableOwnedTagsContainer.AddTag(GameplayTags.Player_Block_InputPressed);
		DebuffEffect->InheritableOwnedTagsContainer.AddTag(GameplayTags.Player_Block_InputReleased);
	}

	DebuffEffect->StackingType = EGameplayEffectStackingType::AggregateByTarget;
	DebuffEffect->StackLimitCount = 1;

	int32 ModifierIndex = DebuffEffect->Modifiers.Num();
	DebuffEffect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& DebuffModifyer = DebuffEffect->Modifiers[ModifierIndex];

	DebuffModifyer.ModifierMagnitude = FScalableFloat(DebuffDamage);
	DebuffModifyer.ModifierOp = EGameplayModOp::Additive;
	DebuffModifyer.Attribute = UAuraAttributeSet::GetInComingDamageAttribute();

	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(DebuffEffect, DebuffContextHandle, 1.f))
	{
		FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(MutableSpec->GetContext().Get());
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable<FGameplayTag>(new FGameplayTag(DamageType));
		AuraContext->SetDamageType(DebuffDamageType);
		AuraContext->SetDebuffDamage(DebuffDamage);
		AuraContext->SetActivateHitReact(false);
		Props.SourceASC->ApplyGameplayEffectSpecToTarget(*MutableSpec, Props.TargetASC);
	}
}

void UAuraAttributeSet::HandleLevelRecovery()
{
	if (bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}
	if (bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

void UAuraAttributeSet::OnRep_PhysicalDamage(const FGameplayAttributeData& OldPhysicalDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalDamage, OldPhysicalDamage);
}

void UAuraAttributeSet::OnRep_Defense(const FGameplayAttributeData& OldDefense) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Defense, OldDefense);
}

void UAuraAttributeSet::OnRep_DefensePenetration(const FGameplayAttributeData& OldDefensePenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, DefensePenetration, OldDefensePenetration);
}

void UAuraAttributeSet::OnRep_MagicalDamage(const FGameplayAttributeData& OldMagicalDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MagicalDamage, OldMagicalDamage);
}

void UAuraAttributeSet::OnRep_Resist(const FGameplayAttributeData& OldResist) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resist, OldResist);
}

void UAuraAttributeSet::OnRep_ResistPenetration(const FGameplayAttributeData& OldResistPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ResistPenetration, OldResistPenetration);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UAuraAttributeSet::OnRep_CritChance(const FGameplayAttributeData& OldCritChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CritChance, OldCritChance);
}

void UAuraAttributeSet::OnRep_CritDamage(const FGameplayAttributeData& OldCritDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CritDamage, OldCritDamage);
}

void UAuraAttributeSet::OnRep_Resist_Fire(const FGameplayAttributeData& OldResist_Fire) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resist_Fire, OldResist_Fire);
}

void UAuraAttributeSet::OnRep_Resist_Lighting(const FGameplayAttributeData& OldResist_Lighting) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resist_Lighting, OldResist_Lighting);
}

void UAuraAttributeSet::OnRep_Resist_Arcane(const FGameplayAttributeData& OldResist_Arcane) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resist_Arcane, OldResist_Arcane);
}
