#ifndef SCHEDULE_IPP
#define SCHEDULE_IPP

#include "PMachine.hpp"

namespace RUNTIME_NAMESPACE {

inline PList<SendQueueItem>&
Scheduler::getSendQueue(Ptr<PMachine> machine) {
    return machine->sendQueue;
}

inline FUNCTION_DECL(Bool, Scheduler::step, (), {
    SchedulerChoice&& chosen = chooseMachine();
    IF(chosen.machine == NULL) 
    THEN({
        RETURN(false);
    }) 
    ELSE({
        IF(chosen.queueIdx < 0) 
        THEN({
            std::cout << chosen.machine << " executes null event" << std::endl;
            chosen.machine->step(chosen.stateIdx, EVENT_NULL);
        }) 
        ELSE({
            SendQueueItem item = getSendQueue(chosen.machine).get(chosen.queueIdx);
            getSendQueue(chosen.machine).removeAt(chosen.queueIdx);
            IF(item.e == EVENT_NEW_MACHINE) 
            THEN({
                std::cout << chosen.machine << " creates " << item.target << std::endl;
                startMachine(item.target, item.payload);
            }) 
            ELSE({
                std::cout << chosen.machine << " sends event " << item.e << " to " << item.target << std::endl;
                item.target->step(chosen.stateIdx, item.e, item.payload);
            })
            ENDIF()
        })
        ENDIF()
        RETURN(true);
    })
    ENDIF()
})

}

#endif