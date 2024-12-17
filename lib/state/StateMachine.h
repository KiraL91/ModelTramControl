#pragma once

#include <L298N.h>

#include "./config.h"
#include "./States.h"

class StateMachine
{
private:
    L298N *_motor;
    State _state;

    static volatile bool isFirstSensorPressed;
    static volatile bool isLastSensorPressed;

    void CalculateCurrentState();

public:
    void Setup(L298N *motor);
    void Run();

    static void Callback();
};

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
    switch (_state)
    {
    case State::UNDEFINED:
        _motor->run(L298N::Direction::FORWARD);
        break;
    case State::FIRST_STOP:
        _motor->run(L298N::Direction::FORWARD);
        break;
    case State::LAST_STOP:
        _motor->run(L298N::Direction::FORWARD);
        break;
    case State::RUNNING:
        _motor->run(L298N::Direction::FORWARD);
        break;
    case State::ERROR:
        _motor->stop();
        break;
    default:
        break;
    }
}

void StateMachine::Callback()
{
    isFirstSensorPressed = PIND & (1 << SENSOR_1_PIN);
    isLastSensorPressed = PIND & (1 << SENSOR_2_PIN);
}

void StateMachine::CalculateCurrentState()
{
    // TODO
    if (_state == State::RUNNING && !isFirstSensorPressed && !isLastSensorPressed)
    {
        _state == State::RUNNING;
        return;
    }
    if (isFirstSensorPressed)
    {
        _state = !isLastSensorPressed ? State::FIRST_STOP : State::ERROR;
    }
    else
    {
        _state = !isFirstSensorPressed ? State::LAST_STOP : State::ERROR;
    }
}
