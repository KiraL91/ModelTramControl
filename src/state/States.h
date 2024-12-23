#pragma once

namespace Model::State
{
    enum class State
    {
        Undefined,
        FirstStop,
        LastStop,
        Fordward,
        Backward,
        Error
    };
}
