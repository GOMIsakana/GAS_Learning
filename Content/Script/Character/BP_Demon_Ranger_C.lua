
local M = UnLua.Class()
local Parent = require "Character.BP_EnemyBase_C"

function M:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)
    self.ActorInfoTitle = "<Title>恶魔</>"
    self.ActorInfoDescription = [[<Damage>敌人</>
<Default>恶魔的一种, 也会让人被打飞, 然后晕厥</>
<Default>但它的方法是扔石头(为什么是扔石头呢? 为什么呢?)</>]]
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