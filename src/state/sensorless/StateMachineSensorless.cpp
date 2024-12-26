#include "./StateMachineSensorless.h"

namespace Model::State::Sensorless
{
    StateMachineSensorless::StateMachineSensorless()
    {
        _state = State::Undefined;
    }

    void StateMachineSensorless::Run() const
    {
        if (!_enabled)
        {
            return;
        }

        LogInfo();

        switch (_state)
        {
        case State::Undefined:
            _state = State::Fordward;
            MotorForward();
            break;
        case State::Fordward:
            if (IsRunning())
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
            if (IsRunning())
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
                ResetTimer();
            }
            break;

        case State::LastStop:
            if (!IsWaitingAtStop())
            {
                _state = State::Backward;
                MotorBackward();
                ResetTimer();
            }
            break;
        default:
            // Indefinido
            break;
        }
    }

    void StateMachineSensorless::Callback()
    {
        // Do nothing
    }

    bool StateMachineSensorless::IsRunning() const
    {
        return millis() - _stateStartTime < TimeRunningMilliseconds;
    }
}
