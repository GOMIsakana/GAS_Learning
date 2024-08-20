local M = UnLua.Class()

local EAttachmentRule = UE.EAttachmentRule
local UKismetSystemLibrary = UE.UKismetSystemLibrary
local UPlayerInterface = UE.UPlayerInterface
local IPlayerInterface = UE.IPlayerInterface

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
    if (self:GetCombatTarget() ~= InCombatTarget) then
        if (self:GetCombatTarget() ~= nil) then
            self:GetCombatTarget():AddEnemyTargetedAmount(-1)
        end
        if (InCombatTarget ~= nil) then
            InCombatTarget:AddEnemyTargetedAmount(1)
        end
    end
    self.Overridden.SetCombatTarget(self, InCombatTarget)
end

function M:Die(DeathImpluse)
    if (self.EnemyArrow ~= nil) then
        self.EnemyArrow:K2_DestroyActor()
        self.EnemyArrow = nil
    end
    if (self:GetCombatTarget() ~= nil) then
        self:GetCombatTarget():AddEnemyTargetedAmount(-1)
    end
    self.Overridden.Die(self, DeathImpluse)
end

return M