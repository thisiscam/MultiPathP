#ifndef RANDOM_SCHEDULER_IPP
#define RANDOM_SCHEDULER_IPP

#include "PMachine.hpp"

namespace RUNTIME_NAMESPACE {

inline FUNCTION_DECL(SchedulerChoice, RandomScheduler::chooseMachine, (), {
    PList<SchedulerChoice> choices;
    FOR(Int i = 0, i < machines.size(), ++i, {
        Ptr<PMachine> machine = machines.get(i);
        // Collect from send queue
        PList<SendQueueItem>& sendQueue = getSendQueue(machine);
        FOR(Int j = 0, j < sendQueue.size(), ++j, {
            SendQueueItem item = sendQueue.get(j);
            IF(item.e == EVENT_NEW_MACHINE) 
            THEN({
                choices.add(SchedulerChoice(machine, j, -1));
                BREAK();
            }) 
            ELSE({
                Int stateIdx = item.target->canServeEvent(item.e);
                IF(stateIdx >= 0) 
                THEN({
                    choices.add(SchedulerChoice(machine, j, stateIdx));
                    BREAK();
                })
                ENDIF()
            })
            ENDIF()
        })
        // Machine is state that can serve null event?
        Int nullStateIdx = machine->canServeEvent(EVENT_NULL);
        IF(nullStateIdx >= 0) 
        THEN({
            choices.add(SchedulerChoice(machine, -1, nullStateIdx));
        })
        ENDIF()
    })
    IF(choices.size() == 0) 
    THEN({
        RETURN(SchedulerChoice(NULL, -1, -1));
    })
    ENDIF()
    // Choose one and remove from send queue
    Int idx = engine.randomInt(choices.size());
    RETURN(choices.get(idx));
})

inline void 
RandomScheduler::startMachine(Ptr<PMachine> machine, const PAny& payload) {
    machines.add(machine);
    machine->start(payload);
}

};

#endif