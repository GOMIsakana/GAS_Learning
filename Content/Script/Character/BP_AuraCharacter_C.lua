local M = UnLua.Class()

local UAbilitySystemBlueprintLibrary = UE.UAbilitySystemBlueprintLibrary
local UBlueprintGameplayTagLibrary = UE.UBlueprintGameplayTagLibrary
local FGameplayTagContainer = UE.FGameplayTagContainer
local FGameplayTag = UE.FGameplayTag

function M:OnEnemyTargetedAmountChange(NewAmount)
    local TagContainer = FGameplayTagContainer()
    local MessageTag = FGameplayTag()
    -- 如果敌人死光光了, 那就放回原来的音乐
    if (NewAmount == 0) then
        self.InCombatState = false
        MessageTag.TagName = "Message.OnDropCombat"
        self.BGMSoundComponent:SwitchSoundMedia(self.DefaultBGM, self.BGMVolumeMultiplier)
    elseif (not self.InCombatState) then
        self.InCombatState = true
        MessageTag.TagName = "Message.OnEnterCombat"
    end

    if (MessageTag.TagName ~= "") then
        UBlueprintGameplayTagLibrary.AddGameplayTag(TagContainer, MessageTag)
        self:SendEffectMessage(TagContainer)
    end
end

function M:AddEnemyTargetedAmount(AmountToAdd)
    self.Overridden.AddEnemyTargetedAmount(self, AmountToAdd)
    self.EnemyTargetedAmount = self.EnemyTargetedAmount + AmountToAdd
    self.OnEnemyTargetedAmountChangeDelegate:Broadcast(self.EnemyTargetedAmount)
end

function M:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)
    self.OnEnemyTargetedAmountChangeDelegate:Add(self, self.OnEnemyTargetedAmountChange)
end

function M:ReceiveDestroyed()
    self.OnEnemyTargetedAmountChangeDelegate:Remove(self, self.OnEnemyTargetedAmountChange)
    self.Overridden.ReceiveDestroyed(self)
end

return M