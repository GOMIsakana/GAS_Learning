
local M = UnLua.Class()
local Parent = require "Character.BP_EnemyBase_C"

local EAttachmentRule = UE.EAttachmentRule
local FHitResult = UE.FHitResult

function M:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)
    self.ActorInfoTitle = "<Title>哥布林弹弓手</>"
    self.ActorInfoDescription = [[<Damage>敌人</>
<Default>视力比较好的哥布林, 使用弹弓进行远程攻击</>
<Default>石头砸人和长矛戳人哪个更痛呢? 大概是一样痛吧</>]]
end

function M:ShowEnemyArrow(ArrowMaterialOverride)
    Parent.ShowEnemyArrow(self, ArrowMaterialOverride)
end

function M:SetCombatTarget(InCombatTarget)
    self.Overridden.SetCombatTarget(self, InCombatTarget)
    self.ShowEnemyArrow(self, nil)
end

return M