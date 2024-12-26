#pragma once

#include <L298N.h>

#include "./State.h"
#include "../config.h"

namespace Model::State
{
    class IStateMachine
    {
    public:
        virtual void Run() const = 0;

        virtual void Setup(L298N *motor);
        virtual State GetState();
        virtual void Enable();
        virtual void Disable();

    protected:
        L298N *_motor;
        mutable State _state;

        bool _enabled = false;
        mutable unsigned long _stateStartTime = 0;

    protected:
        void MotorForward() const;
        void MotorBackward() const;
        void MotorStop() const;
        void ResetTimer() const;
        bool IsWaitingAtStop() const;
        void LogInfo() const;

    private:
        void LogMotorStatus() const;
    };
}
