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
    }

    void StateMachine::Setup(L298N *motor)
    {
        _motor = motor;
    }

    void StateMachine::Run()
    {
        // Verify if in error
        if (isFirstSensorPressed && isLastSensorPressed)
        {
            _state = State::ERROR;
            _motor->stop();
            return;
        }

        switch (_state)
        {
        case State::UNDEFINED:
            Serial.println("UNDEFINED");
            if (isFirstSensorPressed)
            {
                _state = State::FORDWARD;
                _motor->forward();
            }
            if (isLastSensorPressed)
            {
                _state = State::BACKWARD;
                _motor->forward();
            }
            break;
        case State::FORDWARD:
            Serial.println("FORWARD");
            if (isLastSensorPressed)
            {
                _state = State::LAST_STOP;
                _motor->stop();
                _stateStartTime = millis(); // Reiniciar temporizador
            }
            else
            {
                _motor->forward();
            }
            break;
        case State::BACKWARD:
            Serial.println("BACKWARD");
            if (isFirstSensorPressed)
            {
                _state = State::FIRST_STOP;
                _motor->stop();
                _stateStartTime = millis(); // Reiniciar temporizador
            }
            else
            {
                _motor->backward();
            }
            break;
        case State::FIRST_STOP:
            Serial.println("FIRST_STOP");
            if (millis() - _stateStartTime >= STOP_TIME_MILLISECONDS)
            {
                _state = State::FORDWARD;
                _motor->forward();
            }
            break;
        case State::LAST_STOP:
            Serial.println("LAST_STOP");
            if (millis() - _stateStartTime >= STOP_TIME_MILLISECONDS)
            {
                _state = State::BACKWARD;
                _motor->backward();
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
        isFirstSensorPressed = PIND & (1 << SENSOR_1_PIN);
        isLastSensorPressed = PIND & (1 << SENSOR_2_PIN);
    }

}
