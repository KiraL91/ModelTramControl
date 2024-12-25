#include "./StateMachineDual.h"
#include "../State.h"
#include "../../config.h"

namespace Model::State::Dual
{
    volatile bool StateMachineDual::isFirstSensorPressed = false;
    volatile bool StateMachineDual::isLastSensorPressed = false;

    StateMachineDual::StateMachineDual()
    {
        _state = State::Undefined;

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
            _state = State::Error;
            MotorStop();
            return;
        }

        switch (_state)
        {
        case State::Undefined:
            if (isFirstSensorPressed)
            {
                _state = State::Fordward;
                MotorForward();
            }
            if (isLastSensorPressed)
            {
                _state = State::Backward;
                MotorBackward();
            }
            break;
        case State::Fordward:
            if (!isLastSensorPressed)
            {
                MotorForward();
            }
            else
            {
                _state = State::LastStop;
                MotorStop();
                ResetTimer();
            }
            break;
        case State::Backward:
            if (!isFirstSensorPressed)
            {
                MotorBackward();
            }
            else
            {
                _state = State::FirstStop;
                MotorStop();
                ResetTimer();
            }
            break;
        case State::FirstStop:
            if (!IsWaitingAtStop())
            {
                _state = State::Fordward;
                MotorForward();
            }
            break;

        case State::LastStop:
            if (!IsWaitingAtStop())
            {
                _state = State::Backward;
                MotorBackward();
            }
            break;

        case State::Error:
            if (isFirstSensorPressed && !isLastSensorPressed)
            {
                _state = State::Fordward;
            }
            else if (!isFirstSensorPressed && isLastSensorPressed)
            {
                _state = State::Backward;
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
        isFirstSensorPressed = digitalRead(HALL1) == LOW;
        isLastSensorPressed = digitalRead(HALL2) == LOW;
    }
}
