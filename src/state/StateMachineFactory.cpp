#include "./StateMachineFactory.h"
#include "./single/StateMachineSingle.h"
#include "./dual/StateMachineDual.h"
#include "./sensorless/StateMachineSensorless.h"

namespace Model::State
{
    IStateMachine *StateMachineFactory::GetStateMachine()
    {
        IStateMachine *sm = nullptr;

        switch (STOPS)
        {
        case 0:
            sm = new Model::State::Sensorless::StateMachineSensorless();
            break;

        case 1:
            sm = new Model::State::Single::StateMachineSingle();
            break;

        case 2:
            sm = new Model::State::Dual::StateMachineDual();
            break;
        default:
            break;
        }

        return sm;
    }
}
