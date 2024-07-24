// Super Copyright


#include "AbilitySystem/AuraAbilitySystemComponentBase.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Abilities/AuraGameplayAbilityBase.h"
#include "AbilitySystem/Abilities/AuraPassiveAbility.h"
#include "GASAuraGame/AuraLogChannels.h"
#include "Interaction/PlayerInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AuraGameplayTags.h"

void UAuraAbilitySystemComponentBase::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponentBase::ClientOnEffectApplied);

}

void UAuraAbilitySystemComponentBase::AddCharacterAbilitiesFromSaveData(ULoadScreenSaveGame* SaveData)
{
	if (SaveData == nullptr) return;

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	for (const FSavedAbility& SavedAbility : SaveData->SavedAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(SavedAbility.GameplayAbilityClass, SavedAbility.AbilityLevel);
		AbilitySpec.DynamicAbilityTags.AddTag(SavedAbility.AbilitySlot);	// 按键绑定信息
		AbilitySpec.DynamicAbilityTags.AddTag(SavedAbility.AbilityStatus);	// 技能解锁状态信息
		AbilitySpec.SourceObject = GetAvatarActor();
		if (SavedAbility.AbilityType.MatchesTagExact(GameplayTags.Abilities_Type_Offsensive))
		{
			GiveAbility(AbilitySpec);
		}
		else if (SavedAbility.AbilityType.MatchesTagExact(GameplayTags.Abilities_Type_Passive))
		{
			GiveAbility(AbilitySpec);
			if (SavedAbility.AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
		MarkAbilitySpecDirty(AbilitySpec);
	}
	bStartupAbilitiesGiven = true;
	AbilityGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponentBase::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

		if (const UAuraGameplayAbilityBase* AuraAbility = Cast<UAuraGameplayAbilityBase>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}
	bStartupAbilitiesGiven = true;
	AbilityGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponentBase::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartUpPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Equipped);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAuraAbilitySystemComponentBase::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			// ServerSetReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey(), ScopedPredictionKey);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void UAuraAbilitySystemComponentBase::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

// 松开仅做取消能力用
void UAuraAbilitySystemComponentBase::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
			// ServerSetReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey(), ScopedPredictionKey);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void UAuraAbilitySystemComponentBase::ClientOnEffectApplied_Implementation(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}

void UAuraAbilitySystemComponentBase::ForEachAbility(const FForEachAbility& Delegate)
{
	// 锁定技能状态，避免被垃圾回收导致内存访问失败
	FScopedAbilityListLock ActiveScopeLock(*this);
	// 循环每个技能, 并广播到这个委托, 执行对应函数(传入的委托一般是当场创建、当场绑定、当场传入此处执行)
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// 如果传入的委托没有绑定任何函数, 则执行失败爆出错误提示
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura, Error, TEXT("Failed To execute delegate in %hs"), __FUNCTION__)
		}
	}
}

FGameplayTag UAuraAbilitySystemComponentBase::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponentBase::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponentBase::GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponentBase::GetStatusTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetAbilitySpecFromAbilityTag(AbilityTag))
	{
		return GetStatusTagFromSpec(*AbilitySpec);
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponentBase::GetSlotFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetAbilitySpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*AbilitySpec);
	}
	return FGameplayTag();
}

bool UAuraAbilitySystemComponentBase::SlotIsEmpty(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);	// 不加这句可能在执行的时候Spec被回收了，导致报错
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(Spec, Slot))
		{
			return false;
		}
	}
	return true;
}

bool UAuraAbilitySystemComponentBase::AbilityHasSlot(const FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& Slot)
{
	return AbilitySpec.DynamicAbilityTags.HasTagExact(Slot);
}

FGameplayAbilitySpec* UAuraAbilitySystemComponentBase::GetSpecFromSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTagExact(Slot))
		{
			return &Spec;
		}
	}
	return nullptr;
}

bool UAuraAbilitySystemComponentBase::IsPassiveAbility(const FGameplayAbilitySpec& AbilitySpec) const
{
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	const FGameplayTag& AbilityTag = GetAbilityTagFromSpec(AbilitySpec);
	if (AbilityInfo && AbilityTag.IsValid())
	{
		FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
		if (Info.AbilityType.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Type_Passive))
		{
			return true;
		}
	}
	return false;
}

bool UAuraAbilitySystemComponentBase::AbilityHasAnySlot(const FGameplayAbilitySpec& AbilitySpec)
{
	return AbilitySpec.DynamicAbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("InputTag")));
}

void UAuraAbilitySystemComponentBase::AssignAbilityToSlot(FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& Slot)
{
	// 清空技能原来的槽位, 并加上新槽位
	ClearSlot(&AbilitySpec);
	AbilitySpec.DynamicAbilityTags.AddTag(Slot);
}

void UAuraAbilitySystemComponentBase::MulticastActivatePassiveEffect_Implementation(const FGameplayTag& AbilityTag, bool bActivate)
{
	ActivatePassiveAbilityDelegate.Broadcast(AbilityTag, bActivate);
}

FGameplayAbilitySpec* UAuraAbilitySystemComponentBase::GetAbilitySpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability.Get()->AbilityTags.HasTagExact(AbilityTag))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

void UAuraAbilitySystemComponentBase::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

void UAuraAbilitySystemComponentBase::ServerLevelupAbility_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetAbilitySpecFromAbilityTag(AbilityTag))
	{
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToSpellPointsReward(GetAvatarActor(), -1);
		}
		
		FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		FGameplayTag AbilityStatus = GetStatusTagFromSpec(*AbilitySpec);
		if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Abilities_Status_Eligible);
			AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Unlocked);
			AbilityStatus = GameplayTags.Abilities_Status_Unlocked;
		}
		else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked) || AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
		{
			AbilitySpec->Level += 1;
		}
		ClientUpdateAbilityStatus(AbilityTag, AbilityStatus, AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UAuraAbilitySystemComponentBase::UpdateAbilityStatuses(int32 Level)
{
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(this);
	if (!AbilityInfo) return;
	for (FAuraAbilityInfo& Info : AbilityInfo->AbilityInfomation)
	{
		if (!Info.AbilityTag.IsValid()) continue;
		// 从ActibleAbility中检查出未拥有Info中的符合条件的技能时, 赋予该技能
		if (GetAbilitySpecFromAbilityTag(Info.AbilityTag) == nullptr && Level <= Info.LevelRequirement)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1.f);
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Eligible);
			Info.StatusTag = FAuraGameplayTags::Get().Abilities_Status_Eligible;
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityStatus(Info.AbilityTag, FAuraGameplayTags::Get().Abilities_Status_Eligible, 1);
		}
	}
}

void UAuraAbilitySystemComponentBase::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	if (FGameplayAbilitySpec* AbilitySpec = GetAbilitySpecFromAbilityTag(AbilityTag))
	{
		const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		const FGameplayTag& PreviousSlot = GetInputTagFromSpec(*AbilitySpec);
		const FGameplayTag& Status = GetStatusTagFromSpec(*AbilitySpec);

		const bool bStatusValid = Status.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked) || Status.MatchesTagExact(GameplayTags.Abilities_Status_Equipped);
		if (bStatusValid)
		{
			if (!SlotIsEmpty(Slot))	// 当目标槽位有其他技能时 - 此处仅做禁用和清空的逻辑处理
			{
				FGameplayAbilitySpec* AbilitySpecFromSlot = GetSpecFromSlot(Slot);	// 获取目标槽位上的技能
				if (AbilitySpecFromSlot)
				{
					if (AbilityTag.MatchesTagExact(GetAbilityTagFromSpec(*AbilitySpecFromSlot)))	// 如果是两个槽位是同一个技能，则放弃状态转换，提前返回
					{
						ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, Slot, PreviousSlot);
						return;
					}
					
					if (IsPassiveAbility(*AbilitySpecFromSlot)) // 如果原来槽位上的是被动技能, 则禁用它（因为被动技能的生效取决于是否装备）
					{
						DeactivatePassiveAbilityDelegate.Broadcast(GetAbilityTagFromSpec(*AbilitySpecFromSlot));
						MulticastActivatePassiveEffect(GetAbilityTagFromSpec(*AbilitySpecFromSlot), false);
					}

					ClearSlot(AbilitySpecFromSlot); // 清空目标槽位上的技能
					MarkAbilitySpecDirty(*AbilitySpecFromSlot);
				}
			}

			// 对于被动技能, 如果它之前没有装备在任意槽位上, 则尝试激活
			if (!AbilityHasAnySlot(*AbilitySpec))
			{
				if (IsPassiveAbility(*AbilitySpec))
				{
					TryActivateAbility(AbilitySpec->Handle);
					MulticastActivatePassiveEffect(AbilityTag, true);
				}
				AbilitySpec->DynamicAbilityTags.RemoveTag(GetStatusTagFromSpec(*AbilitySpec));
				AbilitySpec->DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Equipped);
			}

			// 交换技能位置(清空技能原来的槽位信息, 赋予它新的槽位信息)
			AssignAbilityToSlot(*AbilitySpec, Slot);
			MarkAbilitySpecDirty(*AbilitySpec);	// 垃圾回收
		}
		ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, Slot, PreviousSlot);
	}
}

void UAuraAbilitySystemComponentBase::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	AbilityEquippedDelegate.Broadcast(AbilityTag, Status, Slot, PreviousSlot);
}

bool UAuraAbilitySystemComponentBase::GetDescriptionByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutDescriptionNextLevel)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetAbilitySpecFromAbilityTag(AbilityTag))
	{
		if (UAuraGameplayAbilityBase* Ability = Cast<UAuraGameplayAbilityBase>(AbilitySpec->Ability))
		{
			OutDescription = Ability->GetDescription(AbilitySpec->Level);
			OutDescriptionNextLevel = Ability->GetDescriptionNextLevel(AbilitySpec->Level + 1);
			if (GetStatusTagFromAbilityTag(AbilityTag) == FAuraGameplayTags::Get().Abilities_Status_Eligible)
			{
				OutDescription = TEXT("<SubTitle>暂未习得</>");
				OutDescriptionNextLevel = Ability->GetDescriptionNextLevel(1);
			}
			return true;
		}
	}
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	if (AbilityInfo == nullptr || !AbilityTag.IsValid() || AbilityTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_None))
	{
		OutDescription = TEXT("<SubTitle>技能不可用</>");
	}
	else
	{
		OutDescription = UAuraGameplayAbilityBase::GetDescriptionLocked(AbilityInfo->FindAbilityInfoByTag(AbilityTag).LevelRequirement);
	}
	OutDescriptionNextLevel = FString();
	return false;
}

void UAuraAbilitySystemComponentBase::ClearSlot(FGameplayAbilitySpec* Spec)
{
	const FGameplayTag Slot = GetInputTagFromSpec(*Spec);
	Spec->DynamicAbilityTags.RemoveTag(Slot);
}

void UAuraAbilitySystemComponentBase::ClearAbilitiesOfSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(&Spec, Slot))
		{
			ClearSlot(&Spec);
		}
	}
}

bool UAuraAbilitySystemComponentBase::AbilityHasSlot(FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& Slot)
{
	return AbilitySpec->DynamicAbilityTags.HasTagExact(Slot);
}

void UAuraAbilitySystemComponentBase::ActivatePassiveAbilities(UAbilitySystemComponent* TargetASC, FDamageModifier& DamageModifier, const FGameplayTag& TriggerTag)
{
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.Ability->AbilityTags.HasTagExact(TriggerTag))	// 仅在目标有指定Tag时触发
		{
			if (UAuraPassiveAbility* PassiveAbility = Cast<UAuraPassiveAbility>(Spec.Ability))
			{
				PassiveAbility->ActivatePassiveAbility(this, TargetASC, DamageModifier, TriggerTag);
			}
		}
	}
}

void UAuraAbilitySystemComponentBase::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePointsReward(GetAvatarActor(), -1);
	}
}

void UAuraAbilitySystemComponentBase::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

void UAuraAbilitySystemComponentBase::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilityGivenDelegate.Broadcast();
	}
}
