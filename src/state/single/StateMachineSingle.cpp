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
    }

    void StateMachineSingle::Setup(L298N *motor) const
    {
        _motor = motor;
    }

    void StateMachineSingle::Enable() const
    {
        _enabled = true;
        _state = MachineState::Undefined;
    }

    void StateMachineSingle::Disable() const
    {
        _enabled = false;
        _state = MachineState::Undefined;
    }

    void StateMachineSingle::Run() const
    {
        if (!_enabled)
        {
            return;
        }

        LogInfo();

        if (isSensorPressed)
        {
            _state = MachineState::Error;
            MotorStop();
            return;
        }

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
            if (!isSensorPressed)
            {
                MotorForward();
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

        default:
            // Indefinido
            break;
        }
    }

    MachineState StateMachineSingle::GetState() const
    {
        return _state;
    }

    void StateMachineSingle::Callback()
    {
        isSensorPressed = digitalRead(PD2) == LOW;
    }

    void StateMachineSingle::MotorForward() const
    {
        _motor->setSpeed(MAX_MOTOR_SPEED);
        _motor->forward();
    }

    void StateMachineSingle::MotorBackward() const
    {
        _motor->setSpeed(MAX_MOTOR_SPEED);
        _motor->backward();
    }

    void StateMachineSingle::MotorStop() const
    {
        _motor->setSpeed(ZERO_MOTOR_SPEED);
        _motor->stop();
    }

    void StateMachineSingle::ResetTimer() const
    {
        _stateStartTime = millis();
    }

    bool StateMachineSingle::IsWaitingAtStop() const
    {
        return millis() - _stateStartTime < STOP_TIME_MILLISECONDS;
    }

    void StateMachineSingle::LogInfo() const
    {
        if (DEBUG_MODE)
        {
            switch (_state)
            {
            case MachineState::Error:
                Serial.println("Error");
                break;
            case MachineState::Fordward:
                Serial.println("Fordward");
                break;
            case MachineState::Backward:
                Serial.println("Backward");
                break;
            case MachineState::FirstStop:
                Serial.println("FirstStop");
                break;
            case MachineState::LastStop:
                Serial.println("LastStop");
                break;
            case MachineState::Undefined:
                Serial.println("Undefined");
                break;
            default:
                break;
            }
        }
    }
}
