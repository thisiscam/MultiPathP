#ifndef PMACHINE_IPP
#define PMACHINE_IPP

#include "ExecutionEngine.hpp"

namespace RUNTIME_NAMESPACE {

inline Bool 
PMachine::randomBool(const string& id) {
    return engine.randomBool(id);
}

};
#endif