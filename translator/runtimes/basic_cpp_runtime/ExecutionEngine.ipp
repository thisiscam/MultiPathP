#ifndef EXECUTION_ENGINE_IPP
#define EXECUTION_ENGINE_IPP

#include "Scheduler.hpp"

namespace basic_cpp_runtime {

inline void 
ExecutionEngine::run(Scheduler& scheduler, PMachine* machine) {
    scheduler.startMachine(machine);
    for(int i = 0; i < maxIteration; i++) {
        if(!scheduler.step()) {
            break;
        }
    }
}

inline bool
ExecutionEngine::randomBool() {
    return true;
}

inline int
ExecutionEngine::randomInt(int max) {
    return 0;
}

}

#endif