#pragma once

#include <L298N.h>

#include "./States.h"

namespace Model::State
{
    class IStateMachine
    {
    public:
        virtual void Setup(L298N *motor) const = 0;
        virtual void Run() const = 0;
        virtual MachineState GetState() const = 0;
        virtual void Enable() const = 0;
        virtual void Disable() const = 0;

    protected:
        mutable L298N *_motor;
        mutable MachineState _state;

        mutable bool _enabled = false;
        mutable unsigned long _stateStartTime = 0;
    };
}
