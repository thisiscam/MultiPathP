#ifndef PMACHINE_IPP
#define PMACHINE_IPP

#include "ExecutionEngine.hpp"

namespace basic_cpp_runtime {

inline bool 
PMachine::randomBool() {
    return engine.randomBool();
}

};
#endif