#include "./IStateMachine.h"

#include <L298N.h>

#include "./State.h"
#include "../config.h"

namespace Model::State
{
    void IStateMachine::Setup(L298N *motor)
    {
        _motor = motor;
    }

    State IStateMachine::GetState()
    {
        return _state;
    }

    void IStateMachine::Enable()
    {
        _enabled = true;
        _state = State::Undefined;
    }

    void IStateMachine::Disable()
    {
        _enabled = false;
        _state = State::Undefined;
    }

    void IStateMachine::MotorForward() const
    {
        _motor->setSpeed(MAX_MOTOR_SPEED);
        _motor->forward();
    }

    void IStateMachine::MotorBackward() const
    {
        _motor->setSpeed(MAX_MOTOR_SPEED);
        _motor->backward();
    }

    void IStateMachine::MotorStop() const
    {
        _motor->setSpeed(ZERO_MOTOR_SPEED);
        _motor->stop();
    }

    void IStateMachine::ResetTimer() const
    {
        _stateStartTime = millis();
    }

    bool IStateMachine::IsWaitingAtStop() const
    {
        return millis() - _stateStartTime < STOP_TIME_MILLISECONDS;
    }

    void IStateMachine::LogInfo() const
    {
        if (DEBUG_MODE)
        {
            switch (_state)
            {
            case State::Error:
                Serial.print("Error. ");
                break;
            case State::Fordward:
                Serial.print("Fordward. ");
                break;
            case State::Backward:
                Serial.print("Backward. ");
                break;
            case State::FirstStop:
                Serial.print("FirstStop. ");
                break;
            case State::LastStop:
                Serial.print("LastStop. ");
                break;
            case State::Undefined:
                Serial.print("Undefined. ");
                break;
            default:
                break;
            }
            LogMotorStatus();
        }
    }

    void IStateMachine::LogMotorStatus() const
    {
        Serial.print("Speed: ");
        Serial.print(_motor->getSpeed());
        Serial.print(", Direction: ");
        Serial.println(_motor->getDirection());
    }
}
