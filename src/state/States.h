#pragma once

namespace Model::State
{
    enum class MachineState
    {
        Undefined,
        FirstStop,
        LastStop,
        Fordward,
        Backward,
        Error
    };
}
