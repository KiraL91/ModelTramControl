#include "./StateMachineSingle.h"
#include "../States.h"
#include "../../config.h"

namespace Model::State::Single
{
    volatile bool StateMachineSingle::isSensorPressed = false;

    StateMachineSingle::StateMachineSingle()
    {
        _state = MachineState::Undefined;

        pinMode(HALL1, INPUT_PULLUP);

        attachInterrupt(
            digitalPinToInterrupt(HALL1),
            Model::State::Single::StateMachineSingle::Callback, CHANGE);
    }

    void StateMachineSingle::Run() const
    {
        if (!_enabled)
        {
            return;
        }

        LogInfo();

        switch (_state)
        {
        case MachineState::Undefined:
            if (isSensorPressed)
            {
                _state = MachineState::Fordward;
                MotorForward();
            }
            break;

        case MachineState::Fordward:
            if (isSensorPressed)
            {
                _state = MachineState::FirstStop;
                MotorStop();
                ResetTimer();
            }
            else
            {
                MotorForward();
            }
            break;

        case MachineState::FirstStop:
            if (!IsWaitingAtStop())
            {
                _state = MachineState::Fordward;
                MotorForward();
            }
            break;

        default:
            // Indefinido
            break;
        }
    }

    void StateMachineSingle::Callback()
    {
        isSensorPressed = digitalRead(PD2) == LOW;
    }
}
