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
}
