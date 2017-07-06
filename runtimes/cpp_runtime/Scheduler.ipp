#ifndef SCHEDULE_IPP
#define SCHEDULE_IPP

#include <typeinfo>
#include "PMachine.hpp"
#include "Helpers.h"

namespace RUNTIME_NAMESPACE {

inline const PList<SendQueueItem>&
Scheduler::getSendQueue(PMachine* machine) {
    return machine->sendQueue;
}

inline SendQueueItem
Scheduler::popSendQueueItem(const Ptr<PMachine>& machine, const Int& index) {
#ifdef USE_VALUE_SUMMARY
    return unaryOp<SendQueueItem>(machine, [&](PMachine* machine) {
        SendQueueItem&& item = machine->sendQueue.get(index);
        machine->sendQueue.removeAt(index);
        return item;
    });
#else
        SendQueueItem&& item = machine->sendQueue.get(index);
        machine->sendQueue.removeAt(index);
        return item;
#endif
}

inline FUNCTION_DECL(Bool, Scheduler::step, ()) {
    SchedulerChoice&& chosen = chooseMachine();
    IF(chosen.machine == NULL) 
    THEN() {
        RETURN(false);
    }
    ELSE() {
        IF(chosen.queueIdx < 0)
        THEN() {
            std::cout << chosen.machine << " executes null event" << std::endl;
            INVOKE(chosen.machine, void, step, (chosen.stateIdx, EVENT_NULL));
        }
        ELSE() {
            SendQueueItem&& item = popSendQueueItem(chosen.machine, chosen.queueIdx);
            IF(item.e == EVENT_NEW_MACHINE)
            THEN() {
                std::cout << Ptr<PMachine>(chosen.machine) << " creates " << Ptr<PMachine>(item.target) << std::endl;
                startMachine(item.target, item.payload);
            }
            ELSE() {
                std::cout << Ptr<PMachine>(chosen.machine) << " sends event " << Int(item.e) << " to " << Ptr<PMachine>(item.target) << std::endl;
                INVOKE(item.target, void, step, (chosen.stateIdx, item.e, item.payload));
            }
            ENDIF()
        }
        ENDIF()
        RETURN(true);
    }
    ENDIF()
}
END_FUNCTION()

}

#endif