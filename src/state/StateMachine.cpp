#include "./StateMachine.h"
#include "./States.h"
#include "../config.h"

namespace Model::State
{
    volatile bool StateMachine::isFirstSensorPressed = false;
    volatile bool StateMachine::isLastSensorPressed = false;

    StateMachine::StateMachine()
    {
        _state = State::Undefined;
    }

    void StateMachine::Setup(L298N *motor)
    {
        _motor = motor;
    }

    void StateMachine::Enable()
    {
        _enabled = true;
        _state = State::Undefined;
    }
    void StateMachine::Disable()
    {
        _enabled = false;
        _state = State::Undefined;
    }

    void StateMachine::Run()
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

    State StateMachine::GetState()
    {
        return _state;
    }

    void StateMachine::Callback()
    {
        isFirstSensorPressed = digitalRead(PD2) == LOW;
        isLastSensorPressed = digitalRead(PD3) == LOW;
    }

    void StateMachine::MotorForward()
    {
        _motor->setSpeed(MAX_MOTOR_SPEED);
        _motor->forward();
    }

    void StateMachine::MotorBackward()
    {
        _motor->setSpeed(MAX_MOTOR_SPEED);
        _motor->backward();
    }

    void StateMachine::MotorStop()
    {
        _motor->setSpeed(ZERO_MOTOR_SPEED);
        _motor->stop();
    }

    void StateMachine::ResetTimer()
    {
        _stateStartTime = millis();
    }

    bool StateMachine::IsWaitingAtStop()
    {
        return millis() - _stateStartTime < STOP_TIME_MILLISECONDS;
    }

    void StateMachine::LogInfo()
    {
        if (DEBUG_MODE)
        {
            switch (_state)
            {
            case State::Error:
                Serial.println("Error");
                break;
            case State::Fordward:
                Serial.println("Fordward");
                break;
            case State::Backward:
                Serial.println("Backward");
                break;
            case State::FirstStop:
                Serial.println("FirstStop");
                break;
            case State::LastStop:
                Serial.println("LastStop");
                break;
            case State::Undefined:
                Serial.println("Undefined");
                break;
            default:
                break;
            }
        }
    }
}
