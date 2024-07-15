// Super Copyright


#include "Player/AuraPlayerState.h"
#include "Character/AuraCharacter.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponentBase>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::SetCombatLevel(int32 NewLevel, bool bBroadcastLevelupNotify)
{
	Level = NewLevel;
	OnLevelChangeDelegate.Broadcast(Level, bBroadcastLevelupNotify);
}

void AAuraPlayerState::SetXP(float NewXP)
{
	XP = NewXP;
	OnXPChangeDelegate.Broadcast(XP, true);
}

void AAuraPlayerState::SetAttributePoint(int32 NewAttributePoint)
{
	AttributePoint = NewAttributePoint;
	OnAttributePointChangeDelegate.Broadcast(AttributePoint, true);
}

void AAuraPlayerState::SetSpellPoint(int32 NewSpellPoint)
{
	SpellPoint = NewSpellPoint;
	OnSpellPointChangeDelegate.Broadcast(SpellPoint, true);
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, XP);
	DOREPLIFETIME(AAuraPlayerState, AttributePoint);
	DOREPLIFETIME(AAuraPlayerState, SpellPoint);
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangeDelegate.Broadcast(Level, true);
}

void AAuraPlayerState::OnRep_XP(float OldXP)
{
	OnXPChangeDelegate.Broadcast(XP, true);
}

void AAuraPlayerState::OnRep_AttributePoint(int32 OldAttributePoint)
{
	OnAttributePointChangeDelegate.Broadcast(AttributePoint, true);
}

void AAuraPlayerState::OnRep_SpellPoint(int32 OldSpellPoint)
{
	OnSpellPointChangeDelegate.Broadcast(SpellPoint, true);
}
