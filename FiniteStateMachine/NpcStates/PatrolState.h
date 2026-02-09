#pragma once

#include <iostream>
#include <ostream>

#include "NpcContext.h"
#include "../StateMachine/State.h"

namespace NpcAi
{
    class PatrolState : public FSM::State<GhostContext>
    {
        void Enter(GhostContext _context) override
        {
            std::cout << "Enter Patrol _State" << std::endl;
        }

        void Exit(GhostContext _context) override
        {
            std::cout << "Exit Patrol _State" << std::endl;
        }
    };
}
