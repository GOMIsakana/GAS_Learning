// Super Copyright


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/HUD/AuraHUD.h"
#include "Game/AuraGameModeBase.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* AuraPlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = AuraPlayerState->GetAbilitySystemComponent();
			UAttributeSet* AS = AuraPlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, AuraPlayerState, ASC, AS);
			return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* AuraPlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = AuraPlayerState->GetAbilitySystemComponent();
			UAttributeSet* AS = AuraPlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, AuraPlayerState, ASC, AS);
			return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AAuraGameModeBase* AuraGamemode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGamemode != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("InitializeDefaultAttributes AuraGamemode != nullptr"));
		AActor* AvatarActor = ASC->GetAvatarActor();
		UCharacterClassInfo* ClassInfo = AuraGamemode->CharacterClassInfo;
		FCharacterClassDefaultInfo ClassDefaultInfo = AuraGamemode->CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
		
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
