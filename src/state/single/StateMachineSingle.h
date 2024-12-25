#pragma once

#include <L298N.h>
#include "../State.h"
#include "../IStateMachine.h"

namespace Model::State::Single
{
    class StateMachineSingle : public IStateMachine
    {
    protected:
        static volatile bool isSensorPressed;

    public:
        StateMachineSingle();

        void Run() const override;
        static void Callback();
    };
}
