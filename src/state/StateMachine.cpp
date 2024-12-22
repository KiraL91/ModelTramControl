#include "./StateMachine.h"
#include "./States.h"
#include "../config.h"

namespace Model
{
    volatile bool StateMachine::isFirstSensorPressed = false;
    volatile bool StateMachine::isLastSensorPressed = false;

    StateMachine::StateMachine()
    {
        _state = State::UNDEFINED;
        Serial.println("State: UNDEFINED");
    }

    void StateMachine::Setup(L298N *motor)
    {
        _motor = motor;
    }

    void StateMachine::Run()
    {
        // PrintSensorStatus();

        // Verify if in error
        if (isFirstSensorPressed && isLastSensorPressed)
        {
            _state = State::ERROR;
            MotorStop();
            return;
        }

        switch (_state)
        {
        case State::UNDEFINED:
            if (isFirstSensorPressed)
            {
                _state = State::FORDWARD;
                MotorForward();
            }
            if (isLastSensorPressed)
            {
                _state = State::BACKWARD;
                MotorBackward();
            }
            break;
        case State::FORDWARD:
            if (!isLastSensorPressed)
            {
                MotorForward();
            }
            else
            {
                Serial.println("State: LAST_STOP");
                _state = State::LAST_STOP;
                MotorStop();
                ResetTimer();
            }
            break;
        case State::BACKWARD:
            if (!isFirstSensorPressed)
            {
                MotorBackward();
            }
            else
            {
                Serial.println("State: FIRST_STOP");
                _state = State::FIRST_STOP;
                MotorStop();
                ResetTimer();
            }
            break;
        case State::FIRST_STOP:
            if (!IsWaitingAtStop())
            {
                _state = State::FORDWARD;
                MotorForward();
            }
            break;

        case State::LAST_STOP:
            if (!IsWaitingAtStop())
            {
                _state = State::BACKWARD;
                MotorBackward();
            }
            break;

        case State::ERROR:
            Serial.println("ERROR");
            if (isFirstSensorPressed && !isLastSensorPressed)
            {
                _state = State::FORDWARD;
            }
            else if (!isFirstSensorPressed && isLastSensorPressed)
            {
                _state = State::BACKWARD;
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

    void StateMachine::PrintSensorStatus()
    {
        Serial.print(String(isFirstSensorPressed));
        Serial.print(", ");
        Serial.println(String(isLastSensorPressed));
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
