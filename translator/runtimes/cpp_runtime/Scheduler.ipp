#ifndef SCHEDULE_IPP
#define SCHEDULE_IPP

#include "PMachine.hpp"
#include "Helpers.h"

namespace RUNTIME_NAMESPACE {

inline PList<SendQueueItem>&
Scheduler::getSendQueue(PMachine* machine) {
    return machine->sendQueue;
}

inline SendQueueItem 
Scheduler::popSendQueueItem(const Ptr<PMachine>& machine, Int index) {
#ifdef USE_VALUE_SUMMARY
    std::cout << "what";
    return unaryOp<SendQueueItem>(machine, [&](PMachine* machine) {
        SendQueueItem item = machine->sendQueue.get(index);
        std::cout << machine->sendQueue;
        std::cout << std::endl;
        machine->sendQueue.removeAt(index);
        std::cout << machine->sendQueue;
        std::cout << std::endl;
        return item;
    });
#else
        SendQueueItem item = machine->sendQueue.get(index);
        machine->sendQueue.removeAt(index);
        return item;
#endif
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
            INVOKE(chosen.machine, void, step, (chosen.stateIdx, EVENT_NULL));
        }) 
        ELSE({
            SendQueueItem item = popSendQueueItem(chosen.machine, chosen.queueIdx);
            IF(item.e == EVENT_NEW_MACHINE) 
            THEN({
                std::cout << chosen.machine << " creates " << item.target << std::endl;
                startMachine(item.target, item.payload);
            }) 
            ELSE({
                std::cout << chosen.machine << " sends event " << item.e << " to " << item.target << std::endl;
                INVOKE(item.target, void, step, (chosen.stateIdx, item.e, item.payload));
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