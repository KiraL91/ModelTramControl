#pragma once

#include <L298N.h>
#include "./States.h"

namespace Model::State
{
    class StateMachine
    {
    private:
        L298N *_motor;
        State _state;

        static volatile bool isFirstSensorPressed;
        static volatile bool isLastSensorPressed;
        unsigned long _stateStartTime = 0;
        bool _enabled = false;

    public:
        StateMachine();

        void Setup(L298N *motor);
        void Run();
        State GetState();
        void Enable();
        void Disable();

        static void Callback();

    private:
        void ResetTimer();
        bool IsWaitingAtStop();

        void MotorForward();
        void MotorBackward();
        void MotorStop();

        void LogInfo();
    };
}
