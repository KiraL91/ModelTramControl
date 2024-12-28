#pragma once

#include "./IStateMachine.h"

namespace Model::State
{
    class StateMachineFactory
    {
    public:
        static IStateMachine *GetStateMachine();
    };
}
