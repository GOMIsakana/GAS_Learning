// Super Copyright


#include "AbilitySystem/Tasks/AbilityTask_TargetDataUnderMouse.h"
#include "Player/AuraPlayerController.h"
#include "AbilitySystemComponent.h"

UAbilityTask_TargetDataUnderMouse* UAbilityTask_TargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UAbilityTask_TargetDataUnderMouse* MyObject = NewAbilityTask<UAbilityTask_TargetDataUnderMouse>(OwningAbility);

	return MyObject;
}

void UAbilityTask_TargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationKey = GetActivationPredictionKey();
		// 绑定了一个委托在[服务器上设置了TargetData时]，当[该事件]发生时, 通知客户端已获得本数据, 然后在能广播时将该数据分发到每一个客户端
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationKey).AddLambda(
			[this](const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
			{
				// 使用来自客户端的缓存
				AbilitySystemComponent.Get()->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
				if (ShouldBroadcastAbilityTaskDelegates())	// 确保本task所属的ability仍然处于激发(active)状态
				{
					OnMouseTargetDataSignature.Broadcast(DataHandle);
				}
			}
		);
		// 服务器是否已经收到了TargetData
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationKey);
		// 如果服务器未获得TargetData，则等待该TargetData到达(避免玩家提前结束Ability, 导致该task失败)
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UAbilityTask_TargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();

	if (APlayerController* Controller = Ability->GetCurrentActorInfo()->PlayerController.Get())
	{
		FHitResult HitResult;
		Controller->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		Data->HitResult = HitResult;
	}
	DataHandle.Add(Data);

	// 将TargetData发送到服务器
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(), 
		DataHandle, 
		FGameplayTag(), 
		AbilitySystemComponent->ScopedPredictionKey	// 这里需要先执行ScopedPrediction, 不然其数据为空
	);
	if (ShouldBroadcastAbilityTaskDelegates())	// 确保本task所属的ability仍然处于激发(active)状态
	{
		OnMouseTargetDataSignature.Broadcast(DataHandle);
	}
}
