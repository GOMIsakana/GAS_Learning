local M = UnLua.Class()

local UAbilitySystemBlueprintLibrary = UE.UAbilitySystemBlueprintLibrary
local UBlueprintGameplayTagLibrary = UE.UBlueprintGameplayTagLibrary
local FGameplayTagContainer = UE.FGameplayTagContainer
local FGameplayTag = UE.FGameplayTag
local UGameplayStatics = UE.UGameplayStatics

function M:OnEnemyTargetedAmountChange(NewAmount)
    local TagContainer = FGameplayTagContainer()
    local MessageTag = FGameplayTag()
    -- 如果敌人死光光了, 那就放回原来的音乐
    if (NewAmount == 0) then
        self.InCombatState = false
        MessageTag.TagName = "Message.OnDropCombat"
        self.BGMSoundComponent:SwitchSoundMedia(self.DefaultNormalBGM, self.BGMVolumeMultiplier)
    elseif (not self.InCombatState) then    -- 如果不处于战斗状态, 且锁定自身的敌人数量大于0, 则进入战斗状态
        self.InCombatState = true
        MessageTag.TagName = "Message.OnEnterCombat"
        self.BGMSoundComponent:SwitchSoundMedia(self.DefaultCombatBGM, self.BGMVolumeMultiplier)
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
    self.OnEnemyTargetedAmountChangeDelegate:Add(self, self.OnEnemyTargetedAmountChange)

    local Gamemode = UGameplayStatics.GetGameMode(self)
    if (Gamemode ~= nil) then
        self.DefaultNormalBGM = Gamemode:GetMapNormalBackgroundMusic(self)
        self.DefaultCombatBGM = Gamemode:GetMapCombatBackgroundMusic(self)
        if (self.DefaultNormalBGM ~= nil) then
            self.BGMSoundComponent:SwitchSoundMedia(self.DefaultNormalBGM, self.BGMVolumeMultiplier)
        end
    end
    self.Overridden.ReceiveBeginPlay(self)
end

function M:ReceiveDestroyed()
    self.OnEnemyTargetedAmountChangeDelegate:Remove(self, self.OnEnemyTargetedAmountChange)
    self.Overridden.ReceiveDestroyed(self)
end

return M