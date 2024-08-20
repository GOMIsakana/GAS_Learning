local M = UnLua.Class()

local UAuraAbilitySystemLibrary = UE.UAuraAbilitySystemLibrary
local ESpawnActorCollisionHandlingMethod = UE.ESpawnActorCollisionHandlingMethod
local UNiagaraFunctionLibrary = UE.UNiagaraFunctionLibrary
local ENCPoolMethod = UE.ENCPoolMethod
local UKismetSystemLibrary = UE.UKismetSystemLibrary
local UAbilitySystemBlueprintLibrary = UE.UAbilitySystemBlueprintLibrary
local UGameplayStatics = UE.UGameplayStatics
local ECollisionResponse = UE.ECollisionResponse

-- 需要做的: 伪装成罐子, 并完善被打破时候执行的逻辑

local function SpawnActorTimer(Self, LootArray)
    local World = Self:GetWorld()
    local AlwaysSpawn = ESpawnActorCollisionHandlingMethod.AlwaysSpawn
    for LootIndex = 1, LootArray:Length() do
        local Loot = LootArray:Get(LootIndex)
        local SpawnedActor = World:SpawnActor(Loot.ItemClass, Self.DropLocation:K2_GetComponentToWorld(), AlwaysSpawn, Self, Self, "Actor.BP_AuraEffectActorBase_C")    -- 生成Actor
        if (SpawnedActor ~= nil and Loot.bLootItemLevelOverride) then           -- 覆写等级
            SpawnedActor.ActorLevel = Self.ActorLevel
        end
        -- 生成掉落物特效
        if (Loot.SpawnSystem ~= nil) then
            UNiagaraFunctionLibrary.SpawnSystemAtLocation(Self, Loot.SpawnSystem, Self.DropLocation:K2_GetComponentLocation(), Self.DropLocation:K2_GetComponentRotation(), Self.DropLocation:K2_GetComponentScale(), true, true, ENCPoolMethod.None, true)
        end
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

    self.StaticMesh:SetCollisionResponseToAllChannels(ECollisionResponse.ECR_Ignore)
    self.bDead = true
    self:MulticastDied()
    self:SetActorHiddenInGame(true)
end

function M:MulticastDied_RPC()
    if (self.HasTrigged == false) then
        self.HasTrigged = true
        self:SpawnLoots()

        -- 生成破坏音效
        if (self.BreakSound ~= nil) then
            UGameplayStatics.PlaySoundAtLocation(self, self.BreakSound, self.DropLocation:K2_GetComponentLocation(), self.DropLocation:K2_GetComponentRotation())
        end
        -- 生成破坏特效
        if (self.BreakSystem ~= nil) then
            UNiagaraFunctionLibrary.SpawnSystemAtLocation(self, self.BreakSystem, self.DropLocation:K2_GetComponentLocation(), self.DropLocation:K2_GetComponentRotation(), self.DropLocation:K2_GetComponentScale(), true, true, ENCPoolMethod.None, true)
        end
    end
end

function M:LoadActor()
    self.Overridden.LoadActor(self)
    if (self.bDead) then
        self.StaticMesh:SetCollisionResponseToAllChannels(ECollisionResponse.ECR_Ignore)
    end
end

function M:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)
    -- self.SpawnLoots()

    local ASC = UAbilitySystemBlueprintLibrary.GetAbilitySystemComponent(self)
    local EffectContextHandle = ASC:MakeEffectContext()
    local EffectSpecHandle = ASC:MakeOutgoingSpec(self.BreakableActorInitializeEffectClass, 1.0, EffectContextHandle)
    ASC:BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle)

    self.StaticMesh:SetCustomDepthStencilValue(251)     -- 设置高亮颜色为蓝色, 提示在GASAuraGame.h里, 实际上的颜色处理在PP_highlight, 改变高光材质的选项在PostProcessVolume里
end

function M:GetAvatarActor()
    return self
end

return M