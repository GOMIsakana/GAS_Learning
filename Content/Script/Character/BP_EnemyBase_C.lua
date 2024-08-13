local M = UnLua.Class()

local EAttachmentRule = UE.EAttachmentRule

function M:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)
end

function M:ShowEnemyArrow(ArrowMaterialOverride)
    self.Overridden.ShowEnemyArrow(self, ArrowMaterialOverride)
    local Target = self.GetCombatTarget(self)
    if (Target ~= nil and self.EnemyArrow == nil) then
        local World = self:GetWorld()
        local TargetTransform = Target:GetTransform()  -- 生成的transform和目标对齐
        local AlwaysSpawn = UE.ESpawnActorCollisionHandlingMethod.AlwaysSpawn
        self.EnemyArrow = World:SpawnActor(self.EnemyArrowClass, AlwaysSpawn, self, self, "Actor.EnemyArrow.BP_EnemyArrow_C")
        if (ArrowMaterialOverride ~= nil) then  -- 覆写材质
            self.EnemyArrow:GetDecal():SetDecalMaterial(ArrowMaterialOverride)
        end
        -- 附加到目标身上
        self.EnemyArrow:K2_AttachToComponent(Target.RootComponent, "", EAttachmentRule.SnapToTarget, EAttachmentRule.SnapToTarget, EAttachmentRule.SnapToTarget, true)
        -- 设置主人为自己
        self.EnemyArrow:SetOwner(self)
    end
end

function M:SetCombatTarget(InCombatTarget)
    self.Overridden.SetCombatTarget(self, InCombatTarget)
    self.ShowEnemyArrow(self, nil)
end

return M