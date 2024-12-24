#include "./StateMachineDual.h"
#include "../States.h"
#include "../../config.h"

namespace Model::State::Dual
{
    volatile bool StateMachineDual::isFirstSensorPressed = false;
    volatile bool StateMachineDual::isLastSensorPressed = false;

    StateMachineDual::StateMachineDual()
    {
        _state = MachineState::Undefined;

        pinMode(HALL1, INPUT_PULLUP);
        pinMode(HALL2, INPUT_PULLUP);

        attachInterrupt(
            digitalPinToInterrupt(HALL1),
            Model::State::Dual::StateMachineDual::Callback, CHANGE);
        attachInterrupt(
            digitalPinToInterrupt(HALL2),
            Model::State::Dual::StateMachineDual::Callback, CHANGE);
    }

    void StateMachineDual::Run() const
    {
        if (!_enabled)
        {
            return;
        }

        LogInfo();

        if (isFirstSensorPressed && isLastSensorPressed)
        {
            _state = MachineState::Error;
            MotorStop();
            return;
        }

        switch (_state)
        {
        case MachineState::Undefined:
            if (isFirstSensorPressed)
            {
                _state = MachineState::Fordward;
                MotorForward();
            }
            if (isLastSensorPressed)
            {
                _state = MachineState::Backward;
                MotorBackward();
            }
            break;
        case MachineState::Fordward:
            if (!isLastSensorPressed)
            {
                MotorForward();
            }
            else
            {
                _state = MachineState::LastStop;
                MotorStop();
                ResetTimer();
            }
            break;
        case MachineState::Backward:
            if (!isFirstSensorPressed)
            {
                MotorBackward();
            }
            else
            {
                _state = MachineState::FirstStop;
                MotorStop();
                ResetTimer();
            }
            break;
        case MachineState::FirstStop:
            if (!IsWaitingAtStop())
            {
                _state = MachineState::Fordward;
                MotorForward();
            }
            break;

        case MachineState::LastStop:
            if (!IsWaitingAtStop())
            {
                _state = MachineState::Backward;
                MotorBackward();
            }
            break;

        case MachineState::Error:
            if (isFirstSensorPressed && !isLastSensorPressed)
            {
                _state = MachineState::Fordward;
            }
            else if (!isFirstSensorPressed && isLastSensorPressed)
            {
                _state = MachineState::Backward;
            }
            else
            {
                // Sigue en error
            }
            break;
        default:
            // Indefinido
            break;
        }
    }

    void StateMachineDual::Callback()
    {
        isFirstSensorPressed = digitalRead(PD2) == LOW;
        isLastSensorPressed = digitalRead(PD3) == LOW;
    }
}
