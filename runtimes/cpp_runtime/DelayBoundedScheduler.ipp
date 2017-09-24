#include "PMachine.hpp"
#include "Helpers.h"

namespace RUNTIME_NAMESPACE {

inline
Ptr<const PList<SendQueueItem>>
DelayBoundedScheduler::getSendQueuePtr(const Ptr<PMachine>& machine) {
#ifdef USE_VALUE_SUMMARY
    return unaryOp<Ptr<const PList<SendQueueItem>>>(machine, [](PMachine* machine) {
        return &getSendQueue(machine);
    });
#else
    return &getSendQueue(machine);
#endif
}

inline FUNCTION_DECL(SchedulerChoice, DelayBoundedScheduler::chooseMachine, ()) {
    PList<SchedulerChoice> choices;
    Int i = 0;
    FOR(int notused = 0, i < machines.size() & choices.size() <= delayBudget, ++i, {
        const Int& machine_idx = (i + currentMachineIndex) % machines.size();
        const Ptr<PMachine>& machine = machines.get(machine_idx);
        // Collect from send queue
        const Ptr<const PList<SendQueueItem>>& sendQueue = getSendQueuePtr(machine);
        FOR(Int j = 0, j < INVOKE(sendQueue, Int, size, ()), ++j, {
            const SendQueueItem& item = INVOKE(sendQueue, SendQueueItem, get, (j));
            const Int& state_idx = INVOKE(item.target, Int, canServeEvent, (item.e));
            IF(state_idx >= 0) 
            THEN() {
                choices.add(SchedulerChoice(machine, j, state_idx));
                BREAK();
            }
            ENDIF()
        })
        ENDFOR()
        IF(choices.size() <= delayBudget) 
        THEN() {
            // Machine is state that can serve null event?
            const Int& null_state_idx = INVOKE(machine, Int, canServeEvent, (EVENT_NULL));
            IF(null_state_idx >= 0) 
            THEN() {
                choices.add(SchedulerChoice(machine, -1, null_state_idx));
            }
            ENDIF()
        }
        ENDIF()
    })
    ENDFOR()

    IF(choices.size() == 0) 
    THEN() {
        RETURN(SchedulerChoice(NULL, -1, -1));
    }
    ENDIF()
    // Choose one and remove from send queue
    Int&& idx = engine.randomInt(choices.size());
    delayBudget = delayBudget - idx;
    currentMachineIndex = (i + currentMachineIndex) % machines.size();
    RETURN(choices.get(idx))
}
END_FUNCTION()

inline void
DelayBoundedScheduler::addMachine(const Ptr<PMachine>& machine, const PAny& payload) {
    machines.add(machine);
}

};
