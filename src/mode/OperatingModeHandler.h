#pragma once

#include "./OperatingMode.h"
#include "../state/StateMachine.h"
#include "../manual/ManualModeHandler.h"

namespace Model::Mode
{
    class OperatingModeHandler
    {
    private:
        Model::State::StateMachine *_stateMachine = nullptr;
        Model::Manual::ManualModeHandler *_manualModeHandler = nullptr;

        OperatingMode _mode = OperatingMode::Auto;

    public:
        OperatingModeHandler(
            Model::State::StateMachine *stateMachine,
            Model::Manual::ManualModeHandler *manualModeHandler);

        void SetMode(OperatingMode mode);
        void Run();
    };
}
