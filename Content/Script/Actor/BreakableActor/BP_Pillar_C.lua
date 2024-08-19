local M = UnLua.Class()

local Parent = require "Actor.BreakableActor.BP_BreakableActorBase_C"

function M:SpawnLoots()
    Parent.SpawnLoots(self)
end

function M:Die(DeathImpulse)
    Parent.Die(self, DeathImpulse)
end

function M:MulticastDied_RPC()
    Parent.MulticastDied_RPC(self)
end

function M:LoadActor()
    Parent.LoadActor(self)
end

function M:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)
    Parent.ReceiveBeginPlay(self)
end

return M