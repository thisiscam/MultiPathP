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

inline VOID_FUNCTION_DECL(PMachine::step, (const Int& stateIndex, const PEvent& e, const PAny& payload)) {
    const PState& state = states.get(stateIndex);
    IF(isGotoTransition(state, e))
    THEN() {
        FOR(Int i = states.size() - 1, i > stateIndex, --i, {
            popState();
        })
        ENDFOR_NC()
    }
    ENDIF()
    retcode = EXECUTE_FINISHED;
    TransitionFunctionPtr&& transitionFn = getTransition(state, e);
    INVOKE_PTR(transitionFn, void, (this, payload));
    EntryFunctionPtr&& entryFn = getTransitionEntry(state, e);
    INVOKE_PTR(entryFn, void, (this, payload));
    engine.incrementTransitionCount(transitionFn);
}
END_VOID_FUNCTION()

};
#endif