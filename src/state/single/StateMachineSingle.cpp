#include "./StateMachineSingle.h"
#include "../State.h"
#include "../../config.h"

namespace Model::State::Single
{
    volatile bool StateMachineSingle::isSensorPressed = false;

    StateMachineSingle::StateMachineSingle()
    {
        _state = State::Undefined;

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
        case State::Undefined:
            if (isSensorPressed)
            {
                _state = State::Fordward;
                MotorForward();
            }
            break;

        case State::Fordward:
            if (isSensorPressed)
            {
                _state = State::FirstStop;
                MotorStop();
                ResetTimer();
            }
            else
            {
                MotorForward();
            }
            break;

        case State::FirstStop:
            if (!IsWaitingAtStop())
            {
                _state = State::Fordward;
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
        isSensorPressed = digitalRead(HALL1) == LOW;
    }
}
