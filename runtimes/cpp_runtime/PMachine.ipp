#ifndef PMACHINE_IPP
#define PMACHINE_IPP

#include "ExecutionEngine.hpp"

namespace RUNTIME_NAMESPACE {

inline Bool 
PMachine::randomBool(const string& id) {
    return engine.randomBool(getName() + std::to_string(this->id) + "_" + id);
}

};
#endif