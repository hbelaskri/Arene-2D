#pragma once
#include "NpcContext.h"

namespace GhostAi
{
    class Conditions
    {
    public:
        static bool ShouldScatter(GhostContext ctx)
        {
            return ctx.currentGlobalMode == GlobalGhostMode::Scatter;
        }

        static bool ShouldChase(GhostContext ctx)
        {
            return ctx.currentGlobalMode == GlobalGhostMode::Chase;
        }

        static bool IsFrightened(GhostContext ctx)
        {
            return ctx.currentGlobalMode == GlobalGhostMode::Frightened;
        }

        static bool IsNotFrightened(GhostContext ctx)
        {
            return ctx.currentGlobalMode != GlobalGhostMode::Frightened;
        }
    };
}