#pragma once

#include <L298N.h>
#include "../States.h"
#include "../IStateMachine.h"

namespace Model::State::Single
{
    class StateMachineSingle : public IStateMachine
    {
    protected:
        static volatile bool isSensorPressed;

    public:
        StateMachineSingle();

        void Setup(L298N *motor) const override;
        void Run() const override;
        MachineState GetState() const override;
        void Enable() const override;
        void Disable() const override;

        static void Callback();

    private:
        void ResetTimer() const;
        bool IsWaitingAtStop() const;

        void MotorForward() const;
        void MotorBackward() const;
        void MotorStop() const;

        void LogInfo() const;
    };
} // namespace Model::State::Single
