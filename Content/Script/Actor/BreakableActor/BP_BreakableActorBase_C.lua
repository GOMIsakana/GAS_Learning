local M = UnLua.Class()

local UAuraAbilitySystemLibrary = UE.UAuraAbilitySystemLibrary
local ESpawnActorCollisionHandlingMethod = UE.ESpawnActorCollisionHandlingMethod
local UNiagaraFunctionLibrary = UE.UNiagaraFunctionLibrary
local ENCPoolMethod = UE.ENCPoolMethod
local UKismetSystemLibrary = UE.UKismetSystemLibrary
local UAbilitySystemBlueprintLibrary = UE.UAbilitySystemBlueprintLibrary

-- 需要做的: 伪装成罐子, 并完善被打破时候执行的逻辑

local function SpawnActorTimer(Self, LootArray)
    local World = Self:GetWorld()
    local AlwaysSpawn = ESpawnActorCollisionHandlingMethod.AlwaysSpawn
    for LootIndex = 1, LootArray:Length() do
        local Loot = LootArray:Get(LootIndex)
        local SpawnedActor = World:SpawnActor(Loot.ItemClass, Self:GetTransform(), AlwaysSpawn, Self, Self, "Actor.BP_AuraEffectActorBase_C")    -- 生成Actor
        if (SpawnedActor ~= nil and Loot.bLootItemLevelOverride) then           -- 覆写等级
            SpawnedActor.ActorLevel = Self.ActorLevel
        end
        -- 生成特效
        UNiagaraFunctionLibrary.SpawnSystemAtLocation(Self, Loot.SpawnSystem, Self:K2_GetActorLocation(), Self:K2_GetActorRotation(), Self:GetActorScale3D(), true, true, ENCPoolMethod.None, true)
        UKismetSystemLibrary.Delay(Self, Self.LootSpawnDelay)
    end
end

function M:SpawnLoots()
    -- 获取掉落物表, 并获取BreakableItemLootItems
    local LootTiers = UAuraAbilitySystemLibrary.GetLootTiers(self)
    -- print("LootTiers is Valid: " .. tostring(LootTiers))
    if (LootTiers ~= nil) then
        local LootArray = LootTiers:GetBreakableActorLootItems()
        coroutine.resume(coroutine.create(SpawnActorTimer), self, LootArray)
    end
    -- 在物品掉落表的范围内, 循环执行生成Actor函数  (这部分使用coroutine)
        -- 生成Actor
        -- 如果掉落表配置了等级覆盖 -> 设置Actor等级
        -- (因为Actor会自动播放生成动画, 所以不需要处理Actor的生成动画)
        -- 在Actor的位置播放生成粒子
        -- 等待一定延迟(用于延迟下一个循环的执行)
end

function M:Die(DeathImpulse)
    self.Overridden.Die(self, DeathImpulse)

    self.bDead = true
    self:SetLifeSpan(3)
    self:MulticastDied()
end

function M:MulticastDied_RPC()
    if (self.HasTrigged == false) then
        self.HasTrigged = true
        self:SpawnLoots()
    end
end

function M:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)
    -- self.SpawnLoots()

    local ASC = UAbilitySystemBlueprintLibrary.GetAbilitySystemComponent(self)
    local EffectContextHandle = ASC:MakeEffectContext()
    local EffectSpecHandle = ASC:MakeOutgoingSpec(self.BreakableActorInitializeEffectClass, 1.0, EffectContextHandle)
    ASC:BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle)
end

return M