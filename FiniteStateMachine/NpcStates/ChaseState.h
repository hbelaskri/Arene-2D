#pragma once
#include <iostream>
#include "NpcContext.h"
#include "../StateMachine/State.h"

class IGhostEntity {
public:
    virtual void SetTargetToPlayer() = 0;
    virtual void SetTargetToCorner() = 0;
    virtual void SetTargetToRandom() = 0;
    virtual void SetSpeed(float speed) = 0;
    virtual void ReverseDirection() = 0; 
    virtual sf::Vector2i GetGridPosition() const = 0;
};

namespace GhostAi
{
    class BaseGhostState : public FSM::State<GhostContext>
    {
    public:
        void Enter(GhostContext ctx) override {
            if (ctx.ghost) ctx.ghost->ReverseDirection();
        }
    };

    class ChaseState : public BaseGhostState
    {
    public:
        void Execute(GhostContext ctx) override
        {
            if (!ctx.ghost) return;

            ctx.ghost->SetTargetToPlayer();
        }
    };

    class ScatterState : public BaseGhostState
    {
    public:
        void Execute(GhostContext ctx) override
        {
            if (!ctx.ghost) return;

            ctx.ghost->SetTargetToCorner();
        }
    };

    class FrightenedState : public BaseGhostState
    {
    public:
        void Enter(GhostContext ctx) override
        {
            BaseGhostState::Enter(ctx);
            if (ctx.ghost) {
                ctx.ghost->SetSpeed(80.0f);
                ctx.ghost->SetTargetToRandom();
            }
            std::cout << "Ghost is scared!" << std::endl;
        }

        void Execute(GhostContext ctx) override
        {
            if (!ctx.ghost) return;

            sf::Vector2i currentPos = ctx.ghost->GetGridPosition();
            if (currentPos == ctx.targetGridPos) {
                ctx.ghost->SetTargetToRandom();
            }
        }

        void Exit(GhostContext ctx) override
        {
            if (ctx.ghost) ctx.ghost->SetSpeed(100.0f);
        }
    };
}