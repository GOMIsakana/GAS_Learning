
local M = UnLua.Class()
local Parent = require "Character.BP_EnemyBase_C"

function M:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)
    self.ActorInfoTitle = "<Title>恶魔</>"
    self.ActorInfoDescription = [[<Damage>敌人</>
<Default>恶魔的一种, 拥有强健的尾巴, 能够把人抽飞、抽到晕厥</>]]
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