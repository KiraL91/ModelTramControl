#pragma once

#include "./OperatingMode.h"
#include "../state/IStateMachine.h"
#include "../manual/ManualModeHandler.h"

namespace Model::Mode
{
    class OperatingModeHandler
    {
    private:
        Model::State::IStateMachine *_stateMachine = nullptr;
        Model::Manual::ManualModeHandler *_manualModeHandler = nullptr;

        OperatingMode _mode = OperatingMode::Undefined;

    public:
        OperatingModeHandler(
            Model::State::IStateMachine *stateMachine,
            Model::Manual::ManualModeHandler *manualModeHandler);

        void SetMode(OperatingMode mode);
        void Run();

    private:
        void LogInfo();
    };
}
