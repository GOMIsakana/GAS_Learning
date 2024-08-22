
local M = UnLua.Class()
local Parent = require "Character.BP_EnemyBase_C"
local ICombatInterface = UE.ICombatInterface

function M:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)
    self.ActorInfoTitle = "<Title>哥布林长矛兵</>"
    self.ActorInfoDescription = [[<Damage>敌人</>
<Default>随处可见的哥布林, 但要小心它手上的长矛, 那可不是闹着玩的</>]]
end

function M:ShowEnemyArrow(ArrowMaterialOverride)
    Parent.ShowEnemyArrow(self, ArrowMaterialOverride)
end

function M:HideEnemyArrow()
    Parent.HideEnemyArrow(self)
end

function M:SetCombatTarget(InCombatTarget)
    Parent.SetCombatTarget(self, InCombatTarget)
end

function M:Die(DeathImpluse)
    Parent.Die(self, DeathImpluse)
end

return M