#ifndef PMACHINE_IPP
#define PMACHINE_IPP

#include "ExecutionEngine.hpp"

namespace RUNTIME_NAMESPACE {

inline Bool 
PMachine::randomBool() {
    return engine.randomBool();
}

};
#endif