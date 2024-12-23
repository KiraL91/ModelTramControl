#include "./OperatingModeHandler.h"
#include "../state/StateMachine.h"
#include "../manual/ManualModeHandler.h"

namespace Model::Mode
{
    OperatingModeHandler::OperatingModeHandler(
        State::StateMachine *stateMachine,
        Manual::ManualModeHandler *manualModeHandler)
    {
        _stateMachine = stateMachine;
        _manualModeHandler = manualModeHandler;
    }

    void OperatingModeHandler::SetMode(OperatingMode mode)
    {
        if (_mode != mode)
        {
            _mode = mode;

            if (_mode == OperatingMode::Auto)
            {
                Serial.println("MODE: AUTO");
                _stateMachine->Enable();
                _manualModeHandler->Disable();
            }
            else
            {
                Serial.println("MODE: MANUAL");
                _stateMachine->Disable();
                _manualModeHandler->Enable();
            }
        }
    }

    void OperatingModeHandler::Run()
    {
        _mode == OperatingMode::Auto
            ? _stateMachine->Run()
            : _manualModeHandler->Run();
    }
}
