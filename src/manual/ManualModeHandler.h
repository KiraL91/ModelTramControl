#pragma once

#include <L298N.h>

namespace Model::Manual
{
    class ManualModeHandler
    {
    private:
        L298N *_motor;
        bool _enabled = false;

    public:
        void Setup(L298N *motor);
        void Run();
        void Enable();
        void Disable();
    };
}
