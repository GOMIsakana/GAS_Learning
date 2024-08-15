
local M = UnLua.Class()
local Parent = require "Character.BP_EnemyBase_C"

function M:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)
    self.ActorInfoTitle = "<Title>食尸鬼</>"
    self.ActorInfoDescription = [[<Damage>敌人</>
<Default>骇人的生物, 体型比哥布林大上不少, 但不知为何只吃你却不吃哥布林</>]]
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