// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Player/AuraPlayerController.h"
#include "Interaction/BackpackInterface.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface, public IBackpackInterface
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/* 玩家接口 */
	virtual void AddToXP_Implementation(float InXP) override;
	virtual float GetXP_Implementation() override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPoints_Implementation() const override;
	virtual int32 FindLevelForXP_Implementation(float InXP) const override;
	virtual void AddToCombatLevel_Implementation(int32 InCombatLevel) override;
	virtual void AddToAttributePointsReward_Implementation(int32 InAttributePoints) override;
	virtual void AddToSpellPointsReward_Implementation(int32 InSpellPoints) override;
	virtual void LevelUp_Implementation() override;
	virtual void ShowMagicCircle_Implementation(UMaterialInterface* InMagicCircleMaterial) override;
	virtual void HideMagicCircle_Implementation() override;
	virtual void SaveProgress_Implementation(const FName& PlayerStartTag) override;
	void SendEffectMessage_Implementation(FGameplayTagContainer TagContainerToSend);
	/* 玩家接口结束 */

	/* 战斗接口 */
	virtual void Die_Implementation(const FVector& DeathImpulse) override;
	virtual int32 GetCombatLevel_Implementation() override;
	/* 战斗接口结束 */

	/* 背包接口开始 */
	virtual void SortBackpackItems_Implementation(bool bAscending = true) override;
	virtual void ExchangeItem_Implementation(int32 SourceItemSlot, int32 TargetItemSlot) override;
	virtual void GetItemAtBackpackSlot_Implementation(FBackpackItem& OutItem, int32 InBackpackSlot) override;
	virtual void SetItemAtBackpackSlot_Implementation(FBackpackItem InItem, int32 InBackpackSlot, bool bRemoveInItemSourceSlotItem = true) override;
	virtual void GetItemAtEquipSlot_Implementation(FBackpackItem& OutItem, int32 InEquipSlot) override;
	virtual void EquipItemToSlot_Implementation(int32 ToEquipItemBackpackSlot, int32 InEquipSlot) override;
	virtual bool PickupItem_Implementation(UPARAM(Ref) FBackpackItem& InItem) override;
	virtual int32 GetBackpackSize_Implementation() override;

	virtual FBackpackItemUpdateSignature& GetBackpackItemUpdateDelegate() override;
	FBackpackItemUpdateSignature BackpackItemUpdateDelegate;
	/* 背包接口结束 */

	AAuraPlayerController* GetAuraPlayerController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;
protected:

	virtual void OnStunTagChanged(FGameplayTag ReceivedTag, int32 NewCount) override;
	virtual void OnRep_IsStunned() override;
	void LoadProgress();

	FTimerHandle DeathTimer;

	UPROPERTY(EditAnywhere)
	float DeathTimerDelay = 5.f;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	virtual void InitAbilityActorInfo() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LevelUpSoundCue;

	UPROPERTY()
	TObjectPtr<AAuraPlayerController> AuraPlayerController;
};
