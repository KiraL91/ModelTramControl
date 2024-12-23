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
            int speed = potRead <= 511
                            ? map(potRead, 0, 511, 255, 0)
                            : map(potRead, 512, 1024, 0, 255);

            _motor->setSpeed(speed);
            potRead <= 511 ? _motor->forward() : _motor->backward();
        }
    }
}
