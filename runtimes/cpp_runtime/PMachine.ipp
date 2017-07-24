#ifndef PMACHINE_IPP
#define PMACHINE_IPP

#include "ExecutionEngine.hpp"

namespace RUNTIME_NAMESPACE {

inline Bool 
PMachine::randomBool(const string& id) {
    return engine.randomBool(std::string(typeid(*this).name()) + std::to_string(this->id) + "_" + id);
}

};
#endif