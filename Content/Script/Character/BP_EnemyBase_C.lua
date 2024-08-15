local M = UnLua.Class()

local EAttachmentRule = UE.EAttachmentRule

function M:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)
end

function M:ShowEnemyArrow(ArrowMaterialOverride)
    self.Overridden.ShowEnemyArrow(self, ArrowMaterialOverride)
    local Target = self.GetCombatTarget(self)
    if (Target ~= nil) then
        if (self.EnemyArrow ~= nil) then
            self.EnemyArrow:K2_DestroyActor()
            self.EnemyArrow = nil
        end
        local World = self:GetWorld()
        local TargetTransform = Target:GetTransform()  -- 生成的transform和目标对齐
        local AlwaysSpawn = UE.ESpawnActorCollisionHandlingMethod.AlwaysSpawn
        self.EnemyArrow = World:SpawnActor(self.EnemyArrowClass, TargetTransform, AlwaysSpawn, self, self, "Actor.EnemyArrow.BP_EnemyArrow_C")
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
end

function M:Die(DeathImpluse)
    self.EnemyArrow:K2_DestroyActor()
    self.EnemyArrow = nil
    self.Overridden.Die(self, DeathImpluse)
end

return M