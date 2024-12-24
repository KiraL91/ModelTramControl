#pragma once

#include <L298N.h>
#include "../States.h"
#include "../IStateMachine.h"

namespace Model::State::Dual
{
    class StateMachineDual : public IStateMachine
    {
    protected:
        static volatile bool isFirstSensorPressed;
        static volatile bool isLastSensorPressed;

    public:
        StateMachineDual();

        void Run() const override;
        static void Callback();
    };
}
