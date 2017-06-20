#ifndef EXECUTION_ENGINE_IPP
#define EXECUTION_ENGINE_IPP

#include "Scheduler.hpp"

namespace RUNTIME_NAMESPACE {

inline void 
ExecutionEngine::run(Scheduler& scheduler, Ptr<PMachine> machine) {
    scheduler.startMachine(machine);
    for(int i = 0; i < maxIteration; ++i) {
        if(!scheduler.step()) {
            break;
        }
    }
}

inline Bool
ExecutionEngine::randomBool() {
    return true;
}

inline Int
ExecutionEngine::randomInt(Int max) {
    return 0;
}

};

#endif