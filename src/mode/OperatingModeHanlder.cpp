#include "./OperatingModeHandler.h"
#include "../state/IStateMachine.h"
#include "../manual/ManualModeHandler.h"
#include "../config.h"

namespace Model::Mode
{
    OperatingModeHandler::OperatingModeHandler(
        State::IStateMachine *stateMachine,
        Manual::ManualModeHandler *manualModeHandler)
    {
        _stateMachine = stateMachine;
        _manualModeHandler = manualModeHandler;
    }

    void OperatingModeHandler::SetMode(OperatingMode mode)
    {
        if (_mode != mode || _mode == OperatingMode::Undefined)
        {
            _mode = mode;
            switch (_mode)
            {
            case OperatingMode::Auto:
                _stateMachine->Enable();
                _manualModeHandler->Disable();
                break;
            case OperatingMode::Manual:
                _stateMachine->Disable();
                _manualModeHandler->Enable();
                break;
            case OperatingMode::Undefined:
                _stateMachine->Disable();
                _manualModeHandler->Disable();
                break;
            default:
                break;
            }

            LogInfo();
        }
    }

    void OperatingModeHandler::Run()
    {
        _mode == OperatingMode::Auto
            ? _stateMachine->Run()
            : _manualModeHandler->Run();
    }

    void OperatingModeHandler::LogInfo()
    {
        if (DEBUG_MODE)
        {
            Serial.print("OperatingMode: ");
            switch (_mode)
            {
            case OperatingMode::Auto:
                Serial.println("Auto");
                break;
            case OperatingMode::Manual:
                Serial.println("Manual");
                break;
            case OperatingMode::Undefined:
                Serial.println("Undefined");
                break;
            default:
                break;
            }
        }
    }
}
