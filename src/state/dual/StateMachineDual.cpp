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
    }

    void StateMachineDual::Setup(L298N *motor) const
    {
        _motor = motor;
    }

    void StateMachineDual::Enable() const
    {
        _enabled = true;
        _state = MachineState::Undefined;
    }
    
    void StateMachineDual::Disable() const
    {
        _enabled = false;
        _state = MachineState::Undefined;
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

    MachineState StateMachineDual::GetState() const
    {
        return _state;
    }

    void StateMachineDual::Callback()
    {
        isFirstSensorPressed = digitalRead(PD2) == LOW;
        isLastSensorPressed = digitalRead(PD3) == LOW;
    }

    void StateMachineDual::MotorForward() const
    {
        _motor->setSpeed(MAX_MOTOR_SPEED);
        _motor->forward();
    }

    void StateMachineDual::MotorBackward() const
    {
        _motor->setSpeed(MAX_MOTOR_SPEED);
        _motor->backward();
    }

    void StateMachineDual::MotorStop() const
    {
        _motor->setSpeed(ZERO_MOTOR_SPEED);
        _motor->stop();
    }

    void StateMachineDual::ResetTimer() const
    {
        _stateStartTime = millis();
    }

    bool StateMachineDual::IsWaitingAtStop() const
    {
        return millis() - _stateStartTime < STOP_TIME_MILLISECONDS;
    }

    void StateMachineDual::LogInfo() const
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
