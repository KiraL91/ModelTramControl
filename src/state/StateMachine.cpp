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
        Serial.println("State: UNDEFINED");
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

        // Verify if in error
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
                Serial.println("State: LAST_STOP");
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
                Serial.println("State: FIRST_STOP");
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
            Serial.println("ERROR");
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
        Serial.println("FORDWARD");
        _motor->setSpeed(MAX_MOTOR_SPEED);
        _motor->forward();
    }

    void StateMachine::MotorBackward()
    {
        Serial.println("BACKWARD");
        _motor->setSpeed(MAX_MOTOR_SPEED);
        _motor->forward();
    }

    void StateMachine::MotorStop()
    {
        Serial.println("STOP");
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
}
