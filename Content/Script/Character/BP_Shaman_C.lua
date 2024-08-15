
local M = UnLua.Class()
local Parent = require "Character.BP_EnemyBase_C"

function M:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)
    self.ActorInfoTitle = "<Title>哥布林萨满</>"
    self.ActorInfoDescription = [[<Damage>敌人</>
<Default>比较聪明的哥布林, 能够使用老家秘传的绝技召唤出恶魔</>]]
end

function M:ShowEnemyArrow(ArrowMaterialOverride)
    self.Overridden.ShowEnemyArrow(self, ArrowMaterialOverride)
    Parent.ShowEnemyArrow(self, ArrowMaterialOverride)
end

function M:SetCombatTarget(InCombatTarget)
    self.Overridden.SetCombatTarget(self, InCombatTarget)
    Parent.SetCombatTarget(self, InCombatTarget)
end

function M:Die(DeathImpluse)
    Parent.Die(self, DeathImpluse)
    self.Overridden.Die(self, DeathImpluse)
end

return M