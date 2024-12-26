#pragma once

#include <L298N.h>
#include "../State.h"
#include "../IStateMachine.h"

namespace Model::State::Sensorless
{
    class StateMachineSensorless : public IStateMachine
    {
    private:
        const unsigned int TimeRunningMilliseconds = 10000;

    public:
        StateMachineSensorless();

        void Run() const override;
        static void Callback();

    private:
        bool IsRunning() const;
    };
}
