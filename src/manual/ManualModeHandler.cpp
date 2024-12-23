#include <L298N.h>
#include "./ManualModeHandler.h"
#include "../config.h"

namespace Model::Manual
{
    void ManualModeHandler::Setup(L298N *motor)
    {
        _motor = motor;
    }

    void ManualModeHandler::Enable()
    {
        _enabled = true;
    }

    void ManualModeHandler::Disable()
    {
        _motor->stop();
        _enabled = false;
    }

    void ManualModeHandler::Run()
    {
        if (_enabled)
        {
            int potRead = analogRead(POT);
            int speed = potRead <= ZERO_POT_VALUE
                            ? map(potRead, MIN_POT_VALUE, ZERO_POT_VALUE, 255, 0)
                            : map(potRead, ZERO_POT_VALUE + 1, MAX_POT_VALUE, 0, 255);

            _motor->setSpeed(speed);
            potRead <= 511 ? _motor->forward() : _motor->backward();

            LogInfo(potRead, speed, _motor->getDirection());
        }
    }

    void ManualModeHandler::LogInfo(int potRead, int speed, int direction)
    {
        if (DEBUG_MODE)
        {
            Serial.print("pot: ");
            Serial.print(potRead);
            Serial.print(", speed: ");
            Serial.print(speed);
            Serial.print(", direction: ");
            Serial.println(direction);
        }
    }
}
