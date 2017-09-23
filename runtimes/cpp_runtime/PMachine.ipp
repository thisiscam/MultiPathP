#ifndef PMACHINE_IPP
#define PMACHINE_IPP

#include "ExecutionEngine.hpp"

namespace RUNTIME_NAMESPACE {

inline Bool 
PMachine::randomBool(const string& id) {
    return engine.randomBool(getName() + std::to_string(this->id) + "_" + id);
}

template<typename M>
Ptr<PMachine> 
PMachine::create(const PAny& payload) {
    Ptr<PMachine>&& machine = PMachine::alloc<M>(engine);
    engine.createMachine(machine);
    INVOKE(machine, void, start, (payload));
    return machine;
}

};
#endif