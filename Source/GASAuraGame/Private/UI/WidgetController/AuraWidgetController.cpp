// Super Copyright


#include "UI/WidgetController/AuraWidgetController.h"
#include "Character/AuraCharacter.h"
#include "Interaction/PlayerInterface.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Game/LoadScreenSaveGame.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCP)
{
	PlayerController = WCP.PlayerController;
	PlayerState = WCP.PlayerState;
	AbilitySystemComponent = WCP.AbilitySystemComponent;
	AttributeSet = WCP.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{

}

void UAuraWidgetController::BindCallbacksToDependencies()
{
}

void UAuraWidgetController::BroadcastAbilityInfo()
{
	if (!GetAuraAbilitySystemComponent()->bStartupAbilitiesGiven)
	{
		return;
	}

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda(
		[this](const FGameplayAbilitySpec& AbilitySpec)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(GetAuraAbilitySystemComponent()->GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = GetAuraAbilitySystemComponent()->GetInputTagFromSpec(AbilitySpec);
			Info.StatusTag = GetAuraAbilitySystemComponent()->GetStatusTagFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		}
	);
	GetAuraAbilitySystemComponent()->ForEachAbility(BroadcastDelegate);
}

AAuraPlayerController* UAuraWidgetController::GetAuraPlayerController()
{
	if (AuraPlayerController == nullptr)
	{
		AuraPlayerController = Cast<AAuraPlayerController>(PlayerController);
	}
	return AuraPlayerController;
}

AAuraPlayerState* UAuraWidgetController::GetAuraPlayerState()
{
	if (AuraPlayerState == nullptr)
	{
		AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	}
	return AuraPlayerState;
}

UAuraAbilitySystemComponentBase* UAuraWidgetController::GetAuraAbilitySystemComponent()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponentBase>(AbilitySystemComponent);
	}
	return AuraAbilitySystemComponent;
}

UAuraAttributeSet* UAuraWidgetController::GetAuraAttributeSet()
{
	if (AuraAttributeSet == nullptr)
	{
		AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	}
	return AuraAttributeSet;
}

void UAuraWidgetController::SaveGameForOwner()
{
	if (AAuraCharacter* OwnerCharacter = Cast<AAuraCharacter>(GetAuraPlayerController()->GetCharacter()))
	{
		if (OwnerCharacter->Implements<UPlayerInterface>())
		{
			AAuraGameModeBase* AuraGamemode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(OwnerCharacter));
			if (AuraGamemode)
			{
				AuraGamemode->SaveWorldState(OwnerCharacter->GetWorld());
				ULoadScreenSaveGame* GameSave = AuraGamemode->RetrieveInGameSaveData();
				IPlayerInterface::Execute_SaveProgress(OwnerCharacter, GameSave->PlayerStartTag);
			}
		}
	}
}

void UAuraWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponentBase* AuraASC)
{
}