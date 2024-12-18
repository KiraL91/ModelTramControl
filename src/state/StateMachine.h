#pragma once

#include <L298N.h>

#include "./States.h"

namespace Model
{
    class StateMachine
    {
    private:
        L298N *_motor;
        State _state;

        static volatile bool isFirstSensorPressed;
        static volatile bool isLastSensorPressed;

        unsigned long _stateStartTime = 0;

    public:
        StateMachine();

        void Setup(L298N *motor);
        void Run();
        State GetState();

        static void Callback();
    };
}
